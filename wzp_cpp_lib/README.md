# 一个简单的C++轮子库，便利一些操作，以下先写上总体概要，后面是各个目录下的详细情况

## 使用：
除开高级容器、多线程读文件、linq、redis、反射、线程池以及与unix底层相关函数外，其他常用功能均只需要
```cpp
#include <wzp_cpp_lib/wzp.hpp>
```

## 总体：
+ 解析配置文件，解析命令行
+ 线程安全的容器、容器的简单算法，单例模式,多例模式,观察者模式的泛型容器,C++11的variant和any, (C++11的Grid以及类似caffe的bubble，并都提供了存储到文件、数据库的方法), 基于二分搜索的集合,字典
+ 获取目录下的文件,文件读取迭代器、多线程读取,path路径处理
+ 函数包装器以及便利函数
+ 轻量级日志模块
+ 类似python的字符串的处理
+ 线程池，Task任务库
+ 类型转换, 类型打印, 定时器, 使用简便的序列化函数
+ 任务注册工厂
+ Redis数据库基于hiredis的C++二次封装，以及基于redis队列的循环任务监听
+ Reflection(C++11轻量级反射), Factory(任意类型工厂模式)
+ Task，链式调用的函数对象
+ 一些unix命令或内核的封装
+ LINQ:C#风格的容器LINQ操作，延迟运算，便利操作的语法糖
+ big_data:一些与大数据采样去重有关的算法

## 分目录情况：
    + config：
        * config_parser.hpp:简单的按行处理的配置文件解析，可以设置注释符号
        * command_parser.hpp:单例模式处理命令行的解析，设置--xx，统一使用这种格式
        * arg_parser.hpp:单例模式解析命令行（微软风格），使用config=xxx的形式
        * click.cpp:arg_parser.hpp与command_parser.hpp的结合改良版，首先定义键值，然后读取config=xxx形式的命令行进行解析，不正确会打印help()
        * wflags.hpp: click.hpp的升级版，可以支持以FLAGS_DEFINE和FLAGS+类型的方式直接调用

    + container:
        * threadsafe_queue.h:一个线程安全模板队列，可以设置处理优先级
        * array_args.hpp：目前只包括获得vector最大值的索引位置,以及任意值的索引返回值,对数组进行洗牌
        * singleton.hpp:单例模式的容器，通过T模板参数来现实化初始不同的类型的单例！
        * smart_singleton.hpp:通过模板参数T来得到不同类型的单例，但是返回的是智能指针！！
        * multiton.hpp:动态类型多例模式容器，通过模版参数T来现实化不同类型的多例，并通过Key（默认字符串）来区分
                也就是说每种类型有一个全局的哈希表，在这种类型的表中使用不同的键值来注册不同的同类对象，便于管理。
        * emaitrx.hpp: 对于高性能矩阵库eigen3的基本封装
        * matrix.hpp:简单的矩阵，包含基本的矩阵运算，主要是与Aiolos配合
        * events.hpp:观察者模式的模板，广播方式。使用运算符进行注册订阅，使用括号运算符通知订阅者，使用-=消除订阅
                注册的是观察者的函数对象，当主题变化时，给定相同的输入参数调用这些函数对象。
        * Variant.hpp:任意类型的容器，支持visit方式访问
        * any.hpp：可以容纳任意类型并进行类型擦出，使用任意类型进行初始化，并且使用任意类型进行转换，缺点是调用值的时候需要显示声明类型，此外如若频繁使用堆内存会有性能开销
        * grid.hpp:一个vector<tuple>的封装，使用时相当于一个列可以是任意类型的矩阵表
        * bubble.hpp:一个四维张量，平铺保存，用来保存同类数据或者图像
        * binary_search.hpp:一个基于二分搜索的集合，可以有数据重复，主要针对数据移动较小的情况，主要是查找而不是增加删除（其实增加删除也还好），在查找为主的情况下可用来替代普通的set，map，更加高效并节省内存。
        * control_safe_queue.h: 简化版但是支持close的线程安全模板队列
    + files:
        * files.hpp:可以获取一个目录下的所有文件名，也可以按照后缀名进行文件获取
        * multireader.hpp:使用多线程分块读取大文件，传入一个对每一行进行处理的单行处理函数，指定返回类型可以将文件内容以vector的形式加载进内存
        * file_loader.hpp:使用next的迭代器形式读取文件，使用regis注册处理每一行的函数对象以及要处理的文件名。
        FileLoad~最新添加的类似STLwrapper的迭代器，可以用C++11的for(auto)结构进行循环迭代
        * path.hpp:使用和python类似的join/basename/splitext等函数
    + function:
        * function_tool.hpp:万能的函数包装器，可以接受任意类型的函数进行处理
                            一个可以链式执行单个输入参数的类型
        * help_function.hpp:提供类似python的range函数，并且可以支持字符等的迭代，使用C++11的for循环使用，极其方便
                        提供类似python的print函数的功能，print(hsj,dhsjh,1,3.5)等等
                        提供一个make_unique方法，更加安全方便，和make_shared逻辑相统一
                        提供任意长度的任意常见类型的max和min，默认返回值是double,如果需要其他需自己指定
                        使用一个reverse_walk包含容器后可以进行反向迭代
        * apply_tuple.hpp:提供找寻tuple中index的函数,提供一个函数包装器，允许将参数以tuple的形式传入，和python类似
    + log:
        * log.hpp:可以设置日志等级的轻量级日志模块，可以设置日志的输出位置，最后会显示时间
    + my_string:
        * string.hpp:包含一些字符串的处理，包括转换字符串到任意类型，分割字符串，join字符串，字符串消除空格, string的format，startwith, end with等
    + thread:
        * thread_pool.hpp:一个安全的线程池，submit任务即可
        * control_safe_thread_pool.hpp:解决之前的线程池不能手动关闭的问题，现在支持关闭之后主线程退出
        * task.hpp:对于async和函数对象的封装，以函数对象初始化，支持链式传递参数，前提是前一个函数是后一个的返回值，不能是void
            ，直到调用run或者get才会开始运行，void返回类型只支持wait和run接口,get接口等效于run，但是会阻塞。
            wait阻塞，get阻塞，run非阻塞。
            ###linux下默认是join,因此必须注意到future的生存期，如果想非阻塞运行的话，需要用一个auto ft = xx.run()，来
            保有这个future对象
        * when_all_any.hpp:对上述的Task任务的简单并行执行，
            WhenAll：输入一组vector的任务，然后调用，get()得到一个vector存放的结果，这里结果必须要一样
            WhenAny:输入一组vector的任务，以及一个TaskFutVec<int>::type fv;来保有一组future对象。
                    make
                    然后得到最先执行结束的那个索引和返回值。
            注意：这两个函数都必须要有返回值，不能是void.
        * task_group.hpp:相当于一个专门处理void返回值的WhenAll，使用run方法将task任务在group中开始执行,
            group中保留了这些task的future, 可以接受Task, lambda, std::bind,vector<Task<void()>>
            调用run之后就开始在别的县城开始执行了，最后可以调用，wait函数阻塞主线程。
        * parallel_algorithm.hpp:提供了一组区间并行算法
            ParallelRange():给一个index的区间，使用上和foreach是一样的，类似于openmp，旨在减少空间成本
            ParallelForeach():和for_each使用方法相同，只是改成了多线程并行执行的版本
            ParallelInvoke()：和task_group一样，接受一组函数对象进行并行执行，比TaskGroup更加方便而已。
            ParalleReduce():类似于分组后的reduce算法（python),逻辑和foreach相同，不过相互之间不独立，
            需要在他们之间寻找最优值
                ParallelReduce(range,init, reduceFunc);
                ParallelReduce(range,init, rangeFunc, reduceFunc);
                输入一个区间和一个初始值，输入最后的reduce函数即可，最后的返回值为reduceFunc的返回值


    + util:
        * lexical_cast.hpp:各种类型的转换包括字符串转为bool类型
        * type_info.hpp：提供了一个简单的将类型连接打印的结构体，主要是学习结构体模板的写法
        * timer.hpp:提供了一个C++11的定时器，可以使用Timer t, t.elapsed()的方法获取来测时间，支持微妙、纳秒、分钟小时等
        * serialize.hpp:提供普通类型、内存连续的结构体、vector<内存连续类型>的序列化为一个字符串的方法，使用简便
                     采用可变参数。
                     序列化时输入一个空string的指针，以及后面可变的内存连续类型
                     反序列化时采用相同的方法，输入一个字符串引用，以及内存连续类型的声明，需要注意的是，
                     序列化和反序列话用的函数要对应，即可变参数类型个数要对应，另外反序列化vector的size要提前准备好（这个比较坑。。。。还没想到更好的办法= =）
        * uuid.hpp:使用UUID()的方法可以获得全球唯一值，在Linux上可能需要-luuid
        * waiter.hpp:可以方便的在不同线程之间公用同一Waiter对象，通过控制num_wait的数量来达到阻塞线程，唤醒线程的作用。
        * random.hpp:封装STL的随机数模块，可以快速指定生成interger/floats/normal/bernoulli等类型的列表，只需声明一个类或者调用一个函数，默认使用random_device真随机数种子
    + executor:
        * executor.hpp:一个最基本的任务调度模型，只包含默认构造函数和一个基本的方法，实际中应根据需要修改源代码，设计具体的接口
        * executor_factory.hpp：一个自动注册的对象工厂，使用宏在头文件中进行注册REGISTER_EXECUTOR(T, key)即可，实际上只是将该类型进行了构造函数的函数对象的注册，然后在使用中使用wzp::factory::produce_unique等进行对象的创建。注意，注册对象构造函数只能在hpp中完成，以免引起连接错误。
    + redis:
        * redis_oper.hpp:对hiredis.h的简单封装，新建对象后提供get/set/rpush/lpop/hash_set/hash_get/hash_delete/del等操作
        * redis_task_listen.hpp:实际是对redis blpop brpop的二次封装，监听队列，然后pop队列调用一个上层所给的handler来处理即可，在使用上建议使用一个智能指针进行包裹后，可以使用多asnyc并行执行等。即封装一个worker类，类中包含比如算法等模块，然后将算法的调用接口封装为函数对象传入即可
    + reflection
        * reflection.hpp:类似于之前的那个注册工厂，但是更加易于使用，重点在于简化factory模式，对于子类使用一个静态变量达到提前注册的目的，然后只需要根据注册的类名进行创建对象即可，目前不适用于模板类
        * factory.hpp:reflection的加强版，可以使用`FactoryRegister(Base, Sub, Args...)`来注册该类的名字，Args为构造函数中需要接受的参数，构造时使用tuple包一下.(保留reflection是为了和以前的代码兼容)
    + task
        * task.hpp:包含一个链式传递函数对象的任务模板，可以设定要链式进行的任务，直到run才会真的执行
    + unix:
        * cmd.hpp:包含类似于php system函数的函数，输入一个字符串指令，子进程执行指令，将结果放入一个vector中，通过返回值bool来
        明确是否执行成功。
        * ip.hpp:添加了一个得到本地IP地址的辅助函数
    + linq:
        *linq.hpp:实际使用只需引入这个文件即可，包含一个from函数将STL迭代器转为Enumarable对象，既保存了一对表示区间的迭代器
        *linq_enumerable.hpp:最基本的保存迭代器的类型，包含select,where,group_by, to_vector,first,count等操作
        *select_iterator.hpp:select迭代器，保存对于指向元素的运算方法
        *where_iterator.hpp:where迭代器，保存是否指向元素符合bool要求
