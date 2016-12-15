#include <string>
#include <numeric>


#include "function/help_function.hpp"
#include "util/timer.hpp"
#include "thread/taskjob.hpp"

#include "files/pipeline_reader.hpp"

using namespace wzp;
using namespace std;

/*help function*/
void PrintThread(int i)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(i));
    cout << std::this_thread::get_id() << " is running..." <<endl;
}

bool check_prime(int x) // 为了体现效果, 该函数故意没有优化.
{
    for (int i = 2; i < x; ++i)
    if (x % i == 0)
        return false;
    return true;
}
/******************/

//test the basic async task
void test_task() {
    print("---------");
    print("test task");
    Task<void()> task1([]()->void{print("test void", 1);});
    task1.wait();
    Task<void()> task1_([]()->void{PrintThread(2000);});
    auto ft = task1_.run();

    Task<int()> task2([]{cout << 1 << endl; return 2; });
    print("task2 output should be 2", task2.get());

    Task<int(int)> task3([](int i){cout << i << endl; return i; });
    print("task3's input is 3, the output should be 3", task3.get(3));

    Task<int(int)> task4([](int i){cout << i << endl; return i+1; });
    auto fut = task4.run(5);
    print("task4 input is 5, output is 6", fut.get());

    Task<int(int)> task([](int i){return i;});
    auto result = task.then([](int a){return a+1;}).then([](int a){return a+2;}).then([](int a){return std::to_string(a);}).get(1);
    print("the input is 1, output should be 4", result);

    print("-------ok-----------");
}

//test whenall function
void test_when_all() {
    print("------------------");
    print("test when all");
    vector<Task<int()>> v = {
        Task<int()>([]{PrintThread(1); std::this_thread::sleep_for(std::chrono::seconds(2)); return 1; }),
        Task<int()>([]{PrintThread(2); return 2; }),
        Task<int()>([]{PrintThread(3); return 3; }),
        Task<int()>([]{PrintThread(4); return 4; })
    };
    print("we are not begin");
    print("when all is begin");
    auto ret_vec = WhenAll(v).get();
    print("the job js OK");
    for(auto ret: ret_vec) {
        print(ret);
    }
    print("-----OK--------");
}


//test when any one finish function
void test_when_any() {
    print("------------------");
    print("test when any");
    vector<Task<int()>> v = {
        Task<int()>([]{PrintThread(1); std::this_thread::sleep_for(std::chrono::seconds(2)); return 1; }),
        Task<int()>([]{PrintThread(2); return 2; }),
        Task<int()>([]{PrintThread(3); return 3; }),
        Task<int()>([]{PrintThread(4); return 4; })
    };
    print("we are not begin");
    print("when any is begin");
    TaskFutVec<int>::type fv;
    WhenAny(v, fv).then([](pair<int, int> result)->int{
        print("the first finnish task is:", result.first, "and its result is: ", result.second);
        return result.second;
    }).then([](int ret){
        print("the result is!!!", ret, "let me print it new line");
    }).run();
    print("----OK-------");
}

//test task group
void test_task_group() {
    print("----------");
    Task<void()> t1([]{PrintThread(100);});
    Task<void()> t2([]{PrintThread(200);});
    Task<void()> t3([]{PrintThread(300);});
    TaskGroupPtr group_ptr(new TaskGroup());
    print("begin");
    group_ptr->run(t1);
    print("t1 begin");
    group_ptr->run(t2);
    print("t2 begin");
    group_ptr->run(t3);
    print("t3 begin");
    group_ptr->run(t1, t2, t3, []{PrintThread(400);});
    print("t4 begin");
    std::vector<Task<void()>> v;
    for(auto i = 0; i < 5; ++i) {
        v.push_back(t1);
    }
    group_ptr->run(v);
    print("vector begin");
    group_ptr->wait();
    print("----ok------");
}

//test parallel foreach algorithm
void test_parallel_foreach() {
    print("---------test parallel foreach----------");
    vector<int> v;
    for (int i = 0; i < 100000; i++)
    {
        v.push_back(i + 1);
    }
    print("use single thread");
    Timer t1;
    std::for_each(begin(v), end(v), check_prime);
    print("single thread use time:", t1.elapsed());
    print("use parallel thread");
    Timer t2;
    ParallelForeach(v.begin(), v.end(), [](int x){check_prime(x);});
    print("parallel use time:", t2.elapsed());
    print("-----------------");
}

//test parallel invoke, just like task group, give a list of functions
void test_parallel_invoke() {
    print("-------test_parallel_invoke------");
    ParallelInvoke([]{PrintThread(100);}, []{PrintThread(200);},
            []{PrintThread(1000);}, []{print("I;m running");});
}

//test parallel reduce
void test_parallel_reduce() {
    print("--------------------");
    vector<string> v;
    v.reserve(10000000);
    for (int i = 0; i < 10000000; i++)
    {
        v.emplace_back(std::to_string(i + 1));
    }

    string init = "";

    auto f = [](const vector<string>::iterator& begin, const vector<string>::iterator& end, string& val)
    {
        return *std::max_element(begin, end, [](string& str1, string& str2){return str1.length()<str2.length(); });
    };

    print("use single thread");
    Timer t1;
    cout<<f(v.begin(), v.end(), init)<<endl;
    print("singe used:", t1.elapsed());
    print("use multi thread");
    Timer t2;
    auto r = ParallelReduce(v, "", f);
    print("time:", t2.elapsed());
    print(r);
    print("-----ok------");

    print("------------");
    std::vector<int> vi;
    vi.reserve(10000000);
    for(int i = 0; i < 10000000; ++i) {
        vi.emplace_back(i / 50000);
    }
    auto foo = [](const vector<int>::iterator& begin, const vector<int>::iterator& end, int init){
        return std::accumulate(begin, end, init);
    };
    print("use one thread");
    Timer t3;
    cout<<foo(vi.begin(), vi.end(), 0)<<endl;
    print("single use:", t3.elapsed());
    print("use multithread");
    Timer t4;
    auto r_ = ParallelReduce(vi, 0, foo);
    print("multi used:", t4.elapsed(), r_);
    print("-----------------");
}

void test_pipeline_reader() {
    //gennerate a big file
    using ll = long long;
    // ofstream out("temp.txt", ios::out);
    // for(ll i = 0; i < 10000000; ++i) {
    //     out<<i % 10000<<endl;
    // }
    // out.close();
    print("-----test pipe line--------");
    print("use single thread");
    ll sum = 0;
    ll sum2 = 0;
    auto add = [&sum](const std::string& line){
        ll i = std::stoll(line);
        sum += i;
    };
    ll i = 0;
    auto add2 = [&sum2, &i](const std::string& line){
        print(i++);
        ll i = std::stoll(line);
        sum2 += i;
    };
    Timer t1;
    ifstream infile("temp.txt", ios::in);
    string line;
    while(std::getline(infile, line)) {
        add(line);
    }
    print("single time used ", t1.elapsed(), sum);
    sum = 0;
    infile.close();
    Timer t2;
    PiplelineReader::read_process("temp.txt", add2);
    print("pipeline", t2.elapsed(), sum2);
}

void print_help() {
    print("test_task", "test_when_all", "test_when_any", "test_task_group",
        "test_parallel_foreach", "test_parallel_invoke", "test_parallel_reduce", "test_pipeline_reader");
}

int main(int argc, char** argv) {
    if(argc != 2) {
        print_help();
        return -1;
    }
    string a(argv[1]);
    if(a == "test_task") {
        test_task();
    }
    else if(a == "test_when_all") {
        test_when_all();
    }
    else if(a == "test_when_any") {
        test_when_any();
    }
    else if(a == "test_task_group") {
        test_task_group();
    }
    else if(a == "test_parallel_foreach") {
        test_parallel_foreach();
    }
    else if(a == "test_parallel_invoke") {
        test_parallel_invoke();
    }
    else if(a == "test_parallel_reduce") {
        test_parallel_reduce();
    }
    else if(a == "test_pipeline_reader") {
        test_pipeline_reader();
    }
    else {
        auto fut = std::async(PrintThread, 100);
        print_help();
    }
    return 0;
}