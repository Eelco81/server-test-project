
pip: 
	pip install -r requirements.txt

clean:
	rm *.xml || :

integration-test : clean
	behave --junit --junit-directory . --color

