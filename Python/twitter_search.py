#!/usr/bin/python
from TwitterSearch import *
import re
import serial
from time import sleep
import random
#If you have a seperate credentials file, uncomment the next line, otherwise you can just edit the secret tokens below.
#from credentials import *

regex = re.compile('[^a-zA-Z]')
#Change this hashtag to what you want to search for
sft = re.compile('#sparkfunthings')
#change this to the port the Arduino is on
myPort = serial.Serial('/dev/ttyACM0', 115200, timeout = 10)
myPort.write("Hello, world!")

tweet_id_list = []
#the messages to be displayed if no tweets are found. You can put as many as you want.
random_messages = ['Random Message One','Random Message Two']
no_message = 0

while True:
    if no_message < 1 : #how many times you want to wait before displaying a stored message
        try:
            print "Searching..."
            tso = TwitterSearchOrder() #Create a Twitter search object
            tso.set_keywords(['#sparkfunthings']) #define the hashtag to search for
            tso.set_include_entities(False) #don't include entity info of the tweet

            #create TwitterSearch object with API secret tokens
            ts = TwitterSearch(
                consumer_key = 'twitter_consumer_key',
                consumer_secret = 'twitter_consumer_secret',
                access_token = 'twitter_access_token',
                access_token_secret = 'twitter_access_token_secret'
                )

            #start the actual search
            for tweet in ts.search_tweets_iterable(tso):
                if tweet['id'] not in tweet_id_list:
                    no_hashtag = sft.sub('', tweet['text'])
                    text_only = regex.sub('', no_hashtag).encode('ascii', 'ignore')+'\r'
                    tweet_id_list.append(tweet['id'])
                    #Prints the entire tweet
                    print text_only
                    #Send entire tweet over serial port
                    myPort.write(text_only)
                    sleep(5)

        except TwitterSearchException as e: #Take care of errors
            print(e)

        no_message +=1
        print "No message counter:" + no_message

    else:
        random_message = random.choice(random_messages)
        print random_message
        myPort.write(random_message)+'\r'
        no_message = 0
        print "No message counter:" + no_message
    sleep(60)

myPort.close()