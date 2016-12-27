#include <iostream>
#include <thread>
#include <set>


// #define LEVELDB

#include "my_string/string.hpp"
#include "log/log.hpp"
#include "container/array_args.hpp"
#include "container/singleton.hpp"
#include "container/multiton.hpp"
#include "container/events.hpp"
#include "container/smart_singleton.hpp"
#include "container/grid.hpp"
#include "container/bubble.hpp"
#include "container/binary_search.hpp"

#ifdef LEVELDB
#include "container/grid_plus.hpp"
#endif

#include "function/function_tool.hpp"
#include "function/help_function.hpp"
#include "util/lexical_cast.hpp"
#include "util/type_info.hpp"
#include "util/timer.hpp"
#include "util/serialize.hpp"
#include "executor1.hpp"
#include "executor2.hpp"
#include "reflection/reflection.hpp"
#include "thread/thread_pool.hpp"
#include "task/task.hpp"
#include "unix/cmd.hpp"

#include "common.h"

using namespace std;
using namespace wzp;

/*help function*/
struct stA {
    int a;
    int b;
    void fun(int x, int y)
    {
        print("class type a and b");
        a = x;
        b = y;
        cout << "a = " << a << " b= " << b << endl;
    }
};

struct Node
{
    int x;
    double y;
};

void print_a_b(int a, int b) { print("print a and b");cout << a << ", " << b << endl; }

template<typename Dtype>
void print_vector(const vector<Dtype>& v) {
    cout<<'(';
    for(auto& d : v) {
        cout<<d<<" ";
    }
    cout<<')'<<std::endl;
}

//test the reflection
class B {
public:
    virtual void fuck() = 0;
};

class D : public B {
public:
    D() {}
    void fuck() override {print("I'm D");}
private:
    static ReflectionRegister(B, D) regis_d;
};

class C : public B {
public:
    C() {}
    void fuck() override {print("I'm C");}
private:
    static ReflectionRegister(B, C) regis_c;
};

ReflectionRegister(B, C) C::regis_c("C");
ReflectionRegister(B, D) D::regis_d("D");

/*help function*/

void fun1(){
    cout<<"void"<<endl;
    // std::this_thread::sleep_for(std::chrono::milliseconds(1500));
}

int l1(int x) {
    return x * 4;
}

int func2(int x, int y){
    return x + y;
}

void test_compose() {
    auto f1 = [](int a){return a + 1; };
    auto g1 = [](int b){return b + 2; };
    auto h1 = [](int c){return c + 3; };
    auto I1 = [](int d){return d + 4; };
    auto J1 = [](int e){return e + 5; };

    auto ret = compose(f1, g1, h1)(3);
    print(ret);
    ret = compose(f1, g1, h1, I1)(3);
    print(ret);
    ret = compose(f1, g1, h1, I1, J1)(3);
    print(ret);
    ret = compose(f1, g1, h1, I1, J1, J1, J1)(3);
    print(ret);
    ret = compose(f1, l1)(3);
    print(ret);
    print("-----------");
}

void test_container() {
    //init
    print("test singleton");
    auto ins_ptr = Singleton<string>::Instance("wzp love you");
    print(ins_ptr->size());
    print(*ins_ptr);
    *ins_ptr = "wzp don't love you";
    print(*(Singleton<string>::GetInstance()));
    Singleton<string>::DestroyInstance();
    print("---------");

    print("test multiton");
    auto wzp = Multiton<string>::Instance("wzp", "love you");
    print(*wzp, wzp->size());
    auto wzp_ = Multiton<string>::Instance("wzp");
    print(*wzp_);
    auto sp = Multiton<int>::Instance("sp", 5);
    *sp = 6;
    auto sp_ = Multiton<int>::Instance("sp");
    print(*sp_);
    print("test ok");
    print("-----------------");

    print("test smart singleton");
    auto smart_ins_ptr = SmartSingleton<string>::Instance("wzp love you");
    print(*smart_ins_ptr);
    smart_ins_ptr->append(" not");
    auto smart_ins_ptr_ = SmartSingleton<string>::GetInstance();
    print(*smart_ins_ptr_, smart_ins_ptr_->size());
    print("---------------");
    print("-----ok------");

    print("test events observer");
    print("------------------");
    using Delegate = std::function<void(int, int)>;
    using Event = Events<Delegate>;
    Event event;
    auto k1 = event+=&print_a_b;
    stA ta;
    auto k2 = event+=[&ta](int a, int b){print("this is lambda"); ta.fun(a, b);};
    auto k3 = event += std::bind(&stA::fun, &ta, std::placeholders::_1, std::placeholders::_2);
    auto k4 = event += [](int a, int b){print("this is add function", a+b);};
    print("##########################");
    event(5, 6);
    print("##########################");
    event -= k1;
    event -= k2;
    print("use disconnect");
    print("##########################");
    event(8, 9);
    print("##########################");
    event.clear();
    event(5, 4);
    print("test observer event ok");
    print("---------------");

    print("########################");
    print("test Grid...");
    Grid<3, int, float, double> grid;
    print("read csv data into the grid");
    grid.from_csv("a.csv");
    auto x = std::get<0>(grid.get_row_at(0));
    auto d = grid.at<0, 2>();
    print("cols:",grid.cols(), "rows:", grid.rows(),
     "capacity:", grid.capacity(), "first data(1):", x,
      "first line third data(0.4):", d);
    grid.print_grid();
    print("change the last line");
    grid.reset_row(3, 1, 1, 1);
    grid.print_grid();
    grid.to_csv("b.csv");
    print("write grid to csv file");
    print("-----------test a complicatied one-------------");
    using block = string;
    Grid<1, int, int, block*> block_grid;
    string s {"假设我是内存中的一大片区域，\
    指向我的指针被放入了brid中的一列，然后得到这个指针，\
    并且打印出来这个内容= ="};
    block_grid.at<0, 2>() = &s;
    cout<<*(block_grid.at<0, 2>())<<endl;
    print("...............");

    print("******************************");
    print("test bubble");
    std::vector<float> v_ = {0.8, 0.7, 0.3, 0.8, 0.7, 0.3,0.8, 0.7, 0.3};
    Bubble<float> b(3, 1, 3, 1);
    b.reshape(3);
    //place data
    b.place(std::move(v_));
    print("save into binary file");
    b.to_bin_file("bubble_bin.dat");

    Bubble<float> a;
    print("=====a is read from binary file========");
    a.from_bin_file("bubble_bin.dat");
    a.reshape(1, 3, 3, 1);
    a.print_bubble();
    print("the read a's shape:", a.num(), a.channel(), a.height(), a.width());
    print("=======================================");
    auto v = std::move(b.get(2));
    for(auto i : v) cout<<i<<" ";
    cout<<endl;
    print("--------------------------------");

    print("********test bubble and Grid*************");
    Grid<1, int, Bubble<float>> gb;
    gb.at<0, 1>().reshape(1,1,3,3);
    gb.at<0, 1>().place(std::move(v_));
    gb.emplace_back(2, b);
    gb.emplace_back(1, std::move(b));
    gb.to_csv("gb.csv");
    gb.clear();
    gb.from_csv("gb.csv");
    gb.at<1, 1>().print_bubble();

    #ifdef LEVELDB
    print("******test leveldb**************");
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "./tmp/grid", &db);
    assert(status.ok());
    // SaveNormalGridToLeveldb("ifd", db, grid);
    //read
    Grid<0, int, float, double> grid_copy;
    ReadNormalGridFromLeveldb("ifd", db, grid_copy);
    grid_copy.print_grid();
    SaveNormalGridToLeveldb("ibubble", db, gb);
    print("this is the bubble read from leveldb");
    gb.clear();
    ReadNormalGridFromLeveldb("ibubble", db, gb);
    gb.at<0, 1>().print_bubble();
    gb.at<1, 1>().print_bubble();
    gb.print_grid();
    print("-----------------------------");
    #endif

    print("*******test Binary Search Container*******");
    vector<int> data;
    data.reserve(1000000);
    for(int i = 1000000; i > 0; --i) {
        data.push_back(i);
    }
    Timer t;
    set<int> normal_set(data.begin(), data.end());
    print("normal create time", t.elapsed_micro());
    t.reset();

    BinSet<int> bin_set(std::move(data));
    print("bin create time", t.elapsed_micro());
    print("-----test find function------");
    t.reset();
    auto normal = normal_set.find(676472);
    print("normal find time", t.elapsed_micro(), *normal);
    t.reset();
    auto bin_ = bin_set.find(676472);
    print("bin find time", t.elapsed_micro(), *bin_);
    print("-------test erase---------");
    t.reset();
    normal_set.erase(676472);
    print("normal erase time", t.elapsed_micro());
    t.reset();
    bin_set.erase(676472);
    print("bin erase time", t.elapsed_micro());
    print("exist 676472 ? ", bin_set.exist(676472));
    print("******test finished********");
}

void test_array(){
    print("-----------");
    print("test array any index");
    vector<int> arrays{1,3,5,7,8};
    print(array_args<int>::any_index(arrays, -3));
    print("----------------");
    print("-------test shuffle array-------");
    print("orriginal:");
    for(auto i : arrays) {
        cout<<i<<" ";
    }
    print("------");
    array_args<int>::shuffle(arrays);
    print("shuflled:");
    for(auto i : arrays) {
        cout<<i<<" ";
    }
    print("-------");
}

void test_string(){
    cout<<"test join string"<<endl;
    vector<string> strs{"4", "fd", "jkjk"};
    cout<<"---------------------"<<endl;
    cout<<"join vector into a string"<<endl;
    cout<<join_string(strs, ',')<<endl;
    cout<<"---------------------"<<endl;
    print("------------------");
    print("--test format string--");
    string str{"it is unhappy {} , {}, {}"};
    print(wzp::format(std::move(str), 1, 2.5, string("wzp")));
    print("------------");
}

void test_log(){
    cout<<"test log"<<endl;
    cout<<"------------"<<endl;
    log::log_init(log_level::Info, log_type::Console, "a.log");
    log::debug("lalala", 3, 4);
    log::info("bibibi", 56, 8.03);
    log::error("hjkahfjhajkfhkj", "sda", 6767);
    log::fatal("fdfdsfs", 11, "sda");
    cout<<"---------------"<<endl;
}

void test_funciton(){
    cout<<"test fucntion wrapper"<<endl;
    cout<<"--------------------"<<endl;
    cout<<function_wrapper(func2, 3, 5)<<endl;
    cout<<"--------------------"<<endl;

    cout<<"test range"<<endl;
    cout<<"-------------"<<endl;
    cout<<"range(15):";
    for(auto i : range(15)) cout<<i<<"  ";
    cout<<endl;

    cout<<"range(2, 6):";
    for(auto i : range(2, 6)) cout<<i<<" ";
    cout<<endl;

    cout << "range(10.5, 15.5):";
    for (float i : range(10.5, 15.5)){
        cout << " " << i;
    }
    cout << endl;
    cout << "range(35,27,-1):";
    for (int i : range(35, 27, -1)){
        cout << " " << i;
    }
    cout << endl;
    cout << "range(2,8,0.5):";
    for (float i : range(2, 8, 0.5)){
        cout << " " << i;
    }
    cout << endl;
    cout << "range(8,7,-0.1):";
    for (auto i : range(8, 7, -0.1)){
        cout << " " << i;
    }
    cout << endl;

    cout << "range('a', 'z'):";
    for (auto i : range('a', 'z'))
    {
        cout << " " << i;
    }
    cout << endl;
    cout<<"-------------"<<endl;
    print("-------------------");
    print("test print", 1, 4.5, "wzp");
    print("----------print ok----------");

    print("test print_err");
    print_err("this is not right");
    print("----------------");

    print("------------");
    print("---test max----");
    print(wzp::max(1, 2.7, 5.7, 4, 8.6f));
    print("------------");
    print("test min");
    print(wzp::min(1.5, 3, 6.7));
    print("---------");

    print("--------------");
    print("--test make unique-");
    auto it = wzp::make_unique<string>("wzp");
    cout<<*it<<endl;
    print("test ok");

    print("------");
    print("test compose function of one arg");
    test_compose();
    print("test ok");

    print("test meta max and meta min");
    auto x = meta_max<int, 1, 3, 5, 2>::value;
    print("meta max of 1, 3, 5, 2:", x);
    x = meta_min<int, 1, 3, 5, 2>::value;
    print("meta min of 1, 3, 5, 2:", x);
}

void test_util(){
    cout<<"test util"<<endl;
    cout<<"-----------"<<endl;
    cout<<lexical_cast<int>(1)<<endl;
    cout << lexical_cast<int>("1") << endl;
    cout << lexical_cast<long>("1") << endl;
    cout << lexical_cast<string>(1) << endl;
    cout << lexical_cast<bool>(1) << endl;
    cout << lexical_cast<double>("1.2") << endl;
    cout << lexical_cast<float>("1.2") << endl;
    print("test from string");
    string s("1.67");
    print(lexical_cast<int>(s));
    print(lexical_cast<float>(s));
    print(lexical_cast<double>(s));
    print(lexical_cast<long long>(s));
    print("+++++++++++");
    s = "true";
    cout << lexical_cast<bool>(s) << endl;
    const char* p = "false";
    cout << lexical_cast<bool>(p) << endl;
    const char* q = "false";
    cout << lexical_cast<bool>(q) << endl;
    cout << lexical_cast<bool>("false") << endl;
    cout<<"-----------"<<endl;

    print("-------------------");
    print("-----test type info------");
    auto str = TypeConnect<int, double>::getName();
    print(str);
    print("--------------------");

    print("--------");
    print("test timer");
    Timer t;
    fun1();
    print(t.elapsed());
    print(t.elapsed_micro());
    print(t.elapsed_nano());
    print(t.elapsed_seconds());
    print(t.elapsed_minutes());
    print(t.elapsed_hours());
    print("----------------");

    print("******************************");
    print("test serialize");
    int x = 672637;
    float f = 1.5f;
    string cache;
    serialize(&cache, x, x, f);
    int y1;
    int y2;
    float y3;
    deserialize(cache, y1, y2, y3);
    print("test normal type:", y1, y2, y3);

    //test struct
    string().swap(cache);
    Node n {123, 0.456};
    x = 900;
    serialize(&cache, n, x);
    Node m;
    x = 0;
    deserialize(cache, m, x);
    print("test struct:", m.x, m.y, x);

    //test vector
    vector<int> v ={1, 2, 3};
    string().swap(cache);
    serialize(&cache, v);
    vector<int> v_(3);
    deserialize(cache, v_);
    print("test vector");
    print_vector(v_);
    print("mix of vector and normal type");
    string().swap(cache);
    x = 500;
    serialize(&cache, x, v, n);
    Node m_{0, 0.0};
    x = 0;
    vector<int> vc(3);
    deserialize(cache, x, vc, m_);
    print("the normal and struct", x, m_.x, m_.y);
    print("the vector");
    print_vector(vc);
    print("test all ok");
    print("=========================================");
}

void test_factory() {
    print("--------------");
    print("test factory");
    auto p = wzp::factory::produce_unique("executor1");
    p->run();
    print("the second");
    auto q = wzp::factory::produce_unique("executor2");
    q ->run();
    print("------");
}

void test_thread_pool() {
    thread_pool pool;
    string s;
    int i = 1;
    while(cin>>s) {
        pool.submit([&s, i]{ print(s, i);});
        pool.submit([&s, i]{ print(s, i * 10);});
        pool.submit([&s, i]{ print(s, i * 100);});
        pool.submit([&s, i]{ print(s, i * 1000);});
        ++i;
    }
}

void test_reflection() {
    print("******test reflection*********");
    auto work = Reflection<B>::create_unique("D");
    work->fuck();
    work = Reflection<B>::create_unique("C");
    work->fuck();
    print("*****************************");
}

void test_task() {
    print("----------");
    print("test list task");
    Task<int(int)> task([](int i){return i;});
    auto result = task.then([](int a){return a+1;}).then([](int a){return a+2;}).then([](int a){return a+3;}).run(1);
    print(result);
    print("-----ok------");
}

void test_unix() {
    print("------test cmd system-------");
    string cmd("ls -l .");
    vector<string> cmd_res;
    wzp::system(cmd, cmd_res);
    for(auto& s : cmd_res) {
        cout<<s<<endl;
    }
    print("---------------------");
}

void print_help(){
    print("test_string", "test_log", "test_funciton", "test_util", "test_array",
        "test_factory", "test_thread_pool", "test_container", "test_task", "test_unix", "test_reflection");
}

int main(int argc, char* argv[]){
    if(argc != 2){
        print_help();
        cerr<<"the input num is not ok"<<endl;
        return -1;
    }
    string a(argv[1]);
    if(a == "test_string"){
        test_string();
    }
    else if(a == "test_log"){
        test_log();
    }
    else if(a == "test_funciton"){
        test_funciton();
    }
    else if(a == "test_util"){
        test_util();
    }
    else if(a == "test_array"){
        test_array();
    }
    else if(a == "test_factory") {
        test_factory();
    }
    else if(a == "test_thread_pool") {
        test_thread_pool();
    }
    else if(a == "test_container") {
        test_container();
    }
    else if(a == "test_task") {
        test_task();
    }
    else if(a == "test_unix") {
        test_unix();
    }
    else if(a == "test_reflection") {
        test_reflection();
    }
    else
    {
        print_help();
        cerr<<"the input is not ok"<<endl;
    }
    cout<<"OKOKOKOKOKO"<<endl;
    return 0;
}
