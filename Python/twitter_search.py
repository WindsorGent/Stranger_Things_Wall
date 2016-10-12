#!/usr/bin/python
from TwitterSearch import *
import re
import serial
from time import sleep
#If you have a seperate credentials file, uncomment the next line, otherwise you can just edit the secret tokens below.
#from credentials import *

regex = re.compile('[^a-zA-Z]')
#Change this hashtag to what you want to search for
sft = re.compile('#sparkfunthings')
#change this to the port the Arduino is on
myPort = serial.Serial('/dev/ttyUSB0', 115200, timeout = 10)
myPort.write("Hello, world!")

tweet_id_list = []

while True:
	try:
		print "Searching..."
		tso = TwitterSearchOrder() # create a TwitterSearchOrder object
		tso.set_keywords(['#sparkfunthings']) # let's define all words we would like to have a look for
		tso.set_include_entities(False) # and don't give us all those entity information

	    # it's about time to create a TwitterSearch object with our secret tokens
		ts = TwitterSearch(
			consumer_key = 'twitter_consumer_key',
			consumer_secret = 'twitter_consumer_secret',
			access_token = 'twitter_access_token',
			access_token_secret = 'twitter_access_token_secret'
		     )

	     # this is where the fun actually starts :)
		for tweet in ts.search_tweets_iterable(tso):
			if tweet['id'] not in tweet_id_list:
				no_hashtag = sft.sub('',  tweet['text'])
				text_only = regex.sub('', no_hashtag).encode('ascii', 'ignore')+'\r'
				tweet_id_list.append(tweet['id'])
                #Prints the entire tweet
				print text_only
                #Send the entire tweet over the serial port, instead of letter by letter
				myPort.write(text_only)
				sleep(5)

	except TwitterSearchException as e: # take care of all those ugly errors if there are some
	    print(e)

	sleep(60)

myPort.close()
