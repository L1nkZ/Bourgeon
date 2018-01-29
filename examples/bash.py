# Plugin code ------------------------------------------------------------------
import bourgeon
import ragnarok_client as client
import os

CMD_PWD = "pwd"
CMD_LS = "ls"
CMD_CD = "cd"
CMD_CAT = "cat"

g_pwd = os.getcwd()

def printf(string):
    client.print_in_chat(string, 0xFFFFFF, 0)


def on_command(chat_buffer):
    global g_pwd
    # pwd
    if chat_buffer.find("/%s" % CMD_PWD) == 0:
        printf("%s $ %s" % (g_pwd, CMD_PWD))
        printf(g_pwd)
    # ls
    elif chat_buffer.find("/%s" % CMD_LS) == 0:
        printf("%s $ %s" % (g_pwd, CMD_LS))
        for node in os.listdir(g_pwd):
            printf(node)
    # cd
    elif chat_buffer.find("/%s" % CMD_CD) == 0:
        parts = chat_buffer.split(" ", 1)
        if len(parts) < 2:
            printf("%s $ %s" % (g_pwd,CMD_CD))
            g_pwd = os.getcwd()
        else:
            dir_name = parts[1]
            printf("%s $ %s %s" % (g_pwd, CMD_CD, dir_name))

            if dir_name == "..":
                pass

            if not os.path.isdir(g_pwd + "\\" + dir_name):
                printf("%s: No such file or directory" % CMD_CD)
                return
            g_pwd += "\\" + dir_name
    #cat
    elif chat_buffer.find("/%s" % CMD_CAT) == 0:
                parts = chat_buffer.split(" ")
                printf("%s $ %s" % (g_pwd, CMD_CAT))
                if len(parts) < 2:
                    return
                for i in range(1, len(parts)):
                    try:
                        f = open(g_pwd + "\\" + parts[i], "r")
                    except FileNotFoundError:
                        printf("%s: %s: No such file or directory" % (CMD_CAT, parts[i]))
                        continue
                    if f:
                        for line in f:
                            printf(line)
                        f.close()


bourgeon.log("> bash.py - The shitty bash plugin")
bourgeon.register_callback("OnTalkType", on_command)
