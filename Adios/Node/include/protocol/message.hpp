#ifndef NODE_MESSAGE_HPP_
#define NODE_MESSAGE_HPP_

#include <string>

#include "protocol/adios.pb.h"

namespace adios {

//the message type , who -> who
enum class MessageType {
    MasterOrder,
    SlaveAnswer
};

using namespace adios_proto;

class Message {
public:
    /*the default constructor*/
    Message() = default;

    Message(MessageType type, const std::string& ip, int port) : m_type(type), m_ip(ip), m_port(port) {}

    //the move constructor
    Message(MessageType type, std::string&& ip, int port) : m_type(type), m_ip(ip), m_port(port) {}

    /*some master messages*/
    //master add a node, give it new node name, and the task queue name it need to listen
    std::string gen_add_node_msg(const std::string& node_name, const std::string& task_queue_name) {
        if(m_type != MessageType::MasterOrder)
            return "";
        adios_proto::Task t;
        set_header(t);
        t.set_task_type(MANAGENODE);
        t.set_node_name(node_name);
        t.set_task_queue_name(task_queue_name);
        std::string str;
        t.SerializeToString(&str);
        return str;
    }

    //the ping message to check the node whether done
    std::string gen_ping_message(int number) {
        //delevery the port to node
        if(m_type != MessageType::MasterOrder)
            return "";
        adios_proto::Task t;
        set_header(t);
        t.set_task_type(PING);
        t.set_check_info(number);
        std::string str;
        t.SerializeToString(&str);
        return str;
    }

    //delevery message to slaves, give the task id and task_name(just like the path of something), if task_name is empty, then ask
    std::string gen_delevery_task(int64_t task_id, const std::string& task_name="") {
        if(m_type != MessageType::MasterOrder)
            return "";
        adios_proto::Task t;
        set_header(t);
        if(task_name.empty()) {
            t.set_task_type(ASK);
        }
        else {
            t.set_task_type(DELEVERY);
            t.set_task_name(task_name);
        }
        t.set_task_id(task_id);
        std::string str;
        t.SerializeToString(&str);
        return str;
    }

    /*the salve message*/
    std::string gen_answer_ping(int number) {
        if(m_type != MessageType::SlaveAnswer) {
            return "";
        }
        adios_proto::Result r;
        set_header(r);
        r.set_answer_type(ANSWER);
        r.set_answer_info(number);
        std::string str;
        r.SerializeToString(&str);
        return str;
    }

    //answer the ask type
    std::string gen_answer_ask(int64_t task_id, const std::string& task_result) {
        if(m_type != MessageType::SlaveAnswer) {
            return "";
        }
        adios_proto::Result r;
        set_header(r);
        r.set_answer_type(ANSWER);
        r.set_task_id(task_id);
        r.set_task_result(task_result);
        std::string str;
        r.SerializeToString(&str);
        return str;
    }

    //upload the client order to master
    std::string gen_upload_cmd(const std::string& cmd) {
        if(m_type != MessageType::SlaveAnswer) {
            return "";
        }
        adios_proto::Result r;
        set_header(r);
        r.set_answer_type(UPLOAD);
        r.set_client_cmd(cmd);
        std::string str;
        r.SerializeToString(&str);
        return str;
    }

private:
    //set the ip, port
    void set_header(adios_proto::Task& t) {
        if(m_type == MessageType::MasterOrder) {
            t.set_master_ip(m_ip);
            t.set_master_port(m_port);
        }
    }

    void set_header(adios_proto::Result& r) {
        if(m_type == MessageType::SlaveAnswer) {
            r.set_slave_ip(m_ip);
            r.set_slave_port(m_port);
        }
    }

private:
    //this message type
    MessageType m_type = MessageType::MasterOrder;

    std::string m_ip = "127.0.0.1";

    int m_port = 9000;
};

} //adios


//set a marco to get the info from the message
#define PARSE(message, req) message.ParseFromString(req)
#define GET_PROTO(message, property) message.property()


#endif /*NODE_MESSAGE_HPP_*/