
template<typename Message_t>
HTTP::MessageDecoder<Message_t>::MessageDecoder () :
    mMessage (nullptr),
    mState (kSearchStart),
    mBodySize (0u)
{
}

template<typename Message_t>
HTTP::MessageDecoder<Message_t>::~MessageDecoder () = default;

template<typename Message_t>
void HTTP::MessageDecoder<Message_t>::Write (const std::string& inData) {
    
    std::istringstream iss (inData);
    
    if (mState == kProcessBody) {
        std::vector<char> body (mBodySize - mBody.size ());
        iss.read (body.data(), body.size ());
        if (!iss) {
            body.resize (iss.gcount ());
        }
        std::copy (body.begin (), body.end (), std::back_inserter (mBody));
        if (mBody.size () >= mBodySize) {
            mMessage->SetBody (mBody);
            this->Done (*mMessage);
            mState = kSearchStart;
        }
    }
    
    for (std::string line; std::getline(iss, line); ) {
        
        switch (mState) {
            case kSearchStart : {
            
                if (MatchStartLine (line)) {
                    mState = kProcessHeaders;
                    mBodySize = 0u;
                    mBody = "";
                }
                
                break;
            }
            case kProcessHeaders : {
                if (line == "\r" || line == "") {
                    const auto contentLength (mMessage->mHeaders.find (Header::CONTENT_LENGTH));
                    if (contentLength == mMessage->mHeaders.end ()) {
                        this->Done (*mMessage);
                        mState = kSearchStart;
                    }
                    else {
                        try {
                           mBodySize = std::stoi (contentLength->second);
                        }
                        catch (...) {
                            mBodySize = 0u;
                            mMessage->mIsValid = false;
                        }
                        if (mBodySize == 0u) {
                            this->Done (*mMessage);
                            mState = kSearchStart;
                        }
                        else {
                            std::vector<char> body (mBodySize);
                            iss.read (body.data(), body.size ());
                            if (!iss) {
                                body.resize (iss.gcount ());
                            }
                            mBody.assign (body.data (), body.size ());
                            if (mBody.size () < mBodySize) {
                                mState = kProcessBody;
                            }
                            else {
                                mMessage->SetBody (mBody);
                                this->Done (*mMessage);
                                mState = kSearchStart;
                            }
                        }
                    }
                }
                else {
                    std::regex reHeader ("^(.+)\\s*:\\s*(.+)\r$");
                    std::smatch matchLine;
                    if (std::regex_search (line, matchLine, reHeader)) {
                        mMessage->mHeaders[matchLine[1].str ()] = matchLine[2].str ();
                    }
                }
                break;
            }
            case kProcessBody : {
                break; // handled in earlier statement
            }
        }
    }
}
