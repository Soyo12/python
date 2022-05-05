from flask import Flask, request
import tools_flask
import sys

app = Flask(__name__)

@app.route('/')
def index():
    return 'Index Page'

@app.route('/hello')
def hello():
    return 'Hello, World'

@app.route('/weworkapi/',methods=['GET','POST'])
def callback():
    if request.method == 'GET':
        msgsignatrue = request.args.get("msg_signature")
        timestamp = request.args.get("timestamp")
        nonce = request.args.get("nonce")
        echostr = request.args.get("echostr")
        is_able = tools_flask.urlvary(msgsignatrue, timestamp, nonce, echostr)
        if is_able:
            final = tools_flask.decode_echo(echostr)
            return final
        else:
            return "e"
    else:
        msgsignatrue = request.args.get("msg_signature")
        timestamp = request.args.get("timestamp")
        nonce = request.args.get("nonce")
        echostr = tools_flask.tiqu(request.data)
        is_able = tools_flask.urlvary(msgsignatrue, timestamp, nonce, echostr)
        if is_able:
            msgs = tools_flask.decode_echo(echostr)
            sender, text = tools_flask.find(msgs.decode("UTF-8"))
            content = "%s发送了：%s" % (tools_flask.dic[sender], text)
            if text == 0:
                return msgs
            tools_flask.todo = content
            tools_flask.ju = True
            app.logger.error(content)
            return msgs
        else:
            tools_flask.send_to("BaiJinFan", msgsignatrue)
            tools_flask.send_to("BaiJinFan", timestamp)
            tools_flask.send_to("BaiJinFan", nonce)
            tools_flask.send_to("BaiJinFan", echostr)
            return "e"
            
@app.route('/wtf/',methods=['GET'])
def wtf():
    situation = int(request.args.get("situation"))
    if situation == 0:
        if tools_flask.ju:
            tools_flask.ju = False
            app.logger.error(tools_flask.did[situation])
            return tools_flask.todo.encode('gb2312')
        else:
            return "ok"
    else:
        tools_flask.send_to("all", tools_flask.did[situation])
        app.logger.error(tools_flask.did[situation])
        return "ok"

if __name__ == '__main__':
    app.debug = True
    app.run()
