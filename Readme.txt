ServerB.cpp (Intermediary Server)
g++ ServerB.cpp -o ServerB -lcurl -pthread
./ServerB

youtube.cpp (contains all youtube functions)

--------------------------------------------

Testing:

Run ServerB
Open seperate terminal for testing with curl commands 
*** you will need to change the IP in the following commands to your IP

Commands for testing youtube function (search by video ID):
(this will retrieve info on one video at a time)

curl 'http://192.168.1.65:8081/youtube?id=Fn_6M1rz_TE'
curl 'http://192.168.1.65:8081/youtube?id=dc8W1XJNsXQ’

Command for testing youtube function (search by channel ID):
(this will retrieve info on all videos in a channel. Currently my channel only has two videos)

curl 'http://192.168.1.65:8081/youtube?channelId=UCB0CpnqE_P4LPHZ0XXZz-Hg'

Commands for testing youtube function (download video by video ID):
(this will download the video to a file on the intermediary server)

curl 'http://192.168.1.65:8081/watch?v=dc8W1XJNsXQ'
curl 'http://192.168.1.65:8081/watch?v=Fn_6M1rz_TE'

Commands for testing youtube function (Retrieve user playlist videos):
(this will retrieve all videos for a specific user)

curl 'http://192.168.1.65:8081/youtube?user=user1'
curl 'http://192.168.1.65:8081/youtube?user=user2'
curl 'http://192.168.1.65:8081/youtube?user=user3'

Command for testing new user account creation:

curl 'http://192.168.1.65:8081/newUser'

Command for testing user authentication / login: 

curl 'http://192.168.1.65:8081/Authorization'

Note:
Currently, the search by video ID works on unlisted videos but the search by channel ID only works on public videos.
I can change this by using OAuth but it requires adding the web server domain as an authorized URI. For now I just left 
the videos public so we can get things working and everyone can test it. Once the web server is setup we can activate OAuth. 
