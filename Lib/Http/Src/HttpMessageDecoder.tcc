
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
            this->sNext.Emit (*mMessage);
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
                    const auto contentLength (mMessage->GetHeaders (Header::CONTENT_LENGTH));
                    if (contentLength.empty ()) {
                        this->sNext.Emit (*mMessage);
                        mState = kSearchStart;
                    }
                    else {
                        try {
                            // \todo: CONTENT_LENGTH is set twice (in constructor and through list). 
                            //        for now take the last one. 
                            mBodySize = std::stoi (contentLength.back ().GetValue ());
                        }
                        catch (...) {
                            mBodySize = 0u;
                            mMessage->mIsValid = false;
                        }
                        if (mBodySize == 0u) {
                            this->sNext.Emit (*mMessage);
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
                                this->sNext.Emit (*mMessage);
                                mState = kSearchStart;
                            }
                        }
                    }
                }
                else {
                    std::regex reHeader ("^(.+)\\s*:\\s*(.+)\r$");
                    std::smatch matchLine;
                    if (std::regex_search (line, matchLine, reHeader)) {
                        mMessage->AddHeader (Header (matchLine[1].str (), matchLine[2].str ()));
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
