import datetime
import random
import string

def getAnswers():
    with open('answers.txt','r') as f:
        for line in f:
            yield line.rstrip()

def outputAnswers():
    with open('outputAnswers.txt','w') as f:
        for answer in getAnswers():
            f.write(formatA(answer))

def id_generator(size=4, chars=string.ascii_uppercase + string.digits):
    return ''.join(random.SystemRandom().choice(string.ascii_uppercase + string.digits) for _ in range(size))

def formatA(answer):
    date = datetime.datetime.now().date()
    time = str(datetime.datetime.now().time()).split('.')[0]
    FORMAT = f'''<dict>
<key>text</key>
<string>{answer}</string>
<key>speaker</key>
<integer>0</integer>
<key>ID</key>
<string>brad_{answer[0:5]}</string>
<key>tokens</key>
<array>
<integer>16</integer>
</array>
<key>modified</key>
<date>{date}T{time}Z</date>
</dict>
'''
    return FORMAT

def getQuestions():
    with open('questions.txt','r') as f:
        for line in f:
            yield line.rstrip()

def outputQuestions():
    ID = 12
    with open('outputQuestions.txt','w') as f:
        for question in getQuestions():
            ID += 1
            f.write(formatQ(question,ID))

def formatQ(question, ID):
    date = datetime.datetime.now().date()
    time = str(datetime.datetime.now().time()).split('.')[0]
    FORMAT = f'''<dict>
<key>text</key>
<string>{question}</string>
<key>ID</key>
<string>Anybody-{ID}</string>
<key>modified</key>
<date>{date}T{time}Z</date>
</dict>
'''
    return FORMAT

def linking():
    while True:
        with open('linking.txt','a') as f:
            qid = input('qid: ')
            aid = input('aid: ')
            link = f'''<dict>
<key>qid</key>
<integer>{qid}</integer>
<key>aid</key>
<integer>{aid}</integer>
<key>value</key>
<integer>6</integer>
</dict>
'''
            f.write(link)
