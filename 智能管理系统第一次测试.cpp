#include<iostream>
#include<string>
#include<vector>//动态数组
#include<algorithm>//提供sort功能
using namespace std;
class Task;
vector<Task*> list;//负责存储、管理所有任务对象
// 辅助函数：将时间字符串(HH:MM)转换为分钟数
int stoint(string time);
//任务基类 - 抽象类，所有具体任务类型继承自此类
class Task
{
public:
    // 记录当前系统中任务的总数
    
    // 构造函数，初始化任务基本信息
    Task(string n,string d,string s,string e):name(n),day(d),start_time(s),end_time(e){num++;}
    //虚析构函数，确保先调用真正子类的析构函数，再调用基类析构函数。
    virtual ~Task(){num--;}//当删除对象时任务数自动减1。
    // 纯虚函数，用于显示任务信息，派生类必须实现
    virtual void show()=0;//通过纯虚函数show()实现多态，不同任务类型有不同的显示格式
    
    // 友元类和函数，允许访问Task的私有成员
    friend class TaskCounter;
    friend struct comparebytime;
    friend bool IsConflict(Task*t1,Task*t2);
    friend istream& operator>>(istream& cin, Task& t);//运算符重载，通过cin直接写入name、day、start_time和end_time
    friend void addorremove();
    friend void searchimportant();
    static int num;	
protected:
    string name;      // 任务名称
    string day;       // 任务日期(格式: YYYY-MM-DD)
    string start_time;// 开始时间(格式: HH:MM)
    string end_time;  // 结束时间(格式: HH:MM)
	
};

// 初始化静态成员变量
int Task::num=0;//静态成员统计任务数量

//课程任务类 子任务类
class CourseTask:public Task{
public:
    string classroom;	
    
    // 无参构造函数
    CourseTask() : Task("", "", "", ""),classroom("") {} 
    // 带参数的构造函数
    CourseTask(string n,string d,string s,string e,string c):Task(n,d,s,e),classroom(c){}
    
    // 实现基类的纯虚函数，显示课程任务信息
    void show(){
        cout << "【课程】" << name <<"  时间:"<<start_time<<'-'<<end_time<< "     教室：" << classroom << endl;
    }	//重写show()，实现差异化的任务信息展示
};


 //考试任务类 -子任务类
class ExamTask :public Task{
public:
    string classroom;	// 考场信息
    
    // 无参构造函数
    ExamTask():Task("", "", "", ""),classroom("") {} 
    // 带参数的构造函数
    ExamTask(string n,string d,string s,string e,string c):Task(n,d,s,e),classroom(c){}
    
    // 实现基类的纯虚函数，显示考试任务信息
    void show(){
        cout << "【考试】" << name  <<"  时间:"<<start_time<<'-'<<end_time<< "     考场：" << classroom << endl;
    }	
};

 // 作业任务类 -子任务类 
class HomeworkTask :public Task{
public:
    string deadline;	// 截止时间
    
    // 默认构造函数
    HomeworkTask():Task("", "", "", ""),deadline("") {} 
    // 带参数的构造函数
    HomeworkTask(string n,string d,string s,string e,string de):Task(n,d,s,e),deadline(de){}
    
    // 实现基类的纯虚函数，显示作业任务信息
    void show(){
        cout << "【课程作业】" << name  <<"  时间:"<<start_time<<'-'<<end_time<<"     截止时间：" << deadline << endl;
    }
};

// 比较器结构体 - 用于按开始时间对任务进行排序
struct comparebytime{                            
    bool operator()(Task*&a1,Task*&a2)const{     //bool用于用于存储逻辑值，可用于判断
        // 将时间字符串转换为分钟数进行比较
        return stoint(a1->start_time) < stoint(a2->start_time);//将时间字符串转换为分钟数并比较，若 a1 的开始时间更早则返回 true
    }
};

//任务的统计和查询类
class TaskCounter {
public:
    TaskCounter() {};//默认构造初始化
    ~TaskCounter() {};//析构
    
    // 按日期搜索任务
    void searchday(string cinday)//枚举找出日期与cinday相同的任务
    {
        ordertask.clear();     // 清空上一个查询任务列表
       
        for(int i=0;i<int(list.size());i++)//遍历全局任务列表 list（以存储的），筛选出日期等于 cinday 的任务
        {                                  
        
            if(list[i]->day==cinday)       //判断是否等于cinday
            {
                ordertask.push_back(list[i]);/*使用 push_back() 将符合条件的任务指针添加到 ordertask ordertask 存储 */
                
            }
        }
        //对符合进行排序
        orderastime();
    }
    
   // 按时间排序并显示 
    void orderastime(){
        sort(ordertask.begin(), ordertask.end(), comparebytime());//按照自定义排序规则排序
        for(Task*i:ordertask){                              
            i->show();                           //多态显示
        }
    }
    
    // 静态成员函数，统计指定月的任务数量
    static void report(int y,int m)
    {
        // 格式化年月字符串
        string year=to_string(y);               //用于将整数类型的年、月转换为字符串格式
        string month=to_string(m);
        if(m<10)
        {
            month="0"+month;                    
        }
        string ym=year+"-"+month;               //实现格式：如2025-06
        
        // 初始化各类任务数量
        int countc= 0;
        int counte= 0;
        int counth= 0;
        
        // 验证输入格式
        if (ym.size()!=7||ym[4]!='-') 
		{      
            cout << "统计失败!"<<endl;
            return;
        }
        
        // 遍历所有任务，统计指定年月的任务数量
        for (Task*t:list)
        {
            if (t->day.substr(0,7)==ym)       /*substr(0, 7) 提取任务日期从0开始长度为7的字串*/
            {
                // 使用dynamic_cast判断任务类型
                if (dynamic_cast<CourseTask*>(t) != nullptr)      ++countc;
                else if (dynamic_cast<ExamTask*>(t) != nullptr)   ++counte;
                else if (dynamic_cast<HomeworkTask*>(t) != nullptr) ++counth;
            }
        }
        
        // 输出统计结果
        cout << " 月度任务统计 ——"<<endl;
        cout << "  课程任务   ：" << countc<< "条"<<endl;
        cout << "  考试任务   ：" << counte<< "条"<<endl;
        cout << "  作业任务   ：" << counth<< "条"<<endl;
        cout << "———————————————"<<endl;
    }
    
    vector<Task*>ordertask;//存储任务指针的动态数组，存储筛选后的符合条件的任务子集
};

// 重载输入运算符，用于输入任务基本信息
istream& operator>>(istream& cin, Task& t) 
{
    cout << "请输入任务名称（可以包含空格）：" << endl;
    cin.ignore();               // 忽略之前的输入缓冲区
    getline(cin, t.name);       // 读取整行任务名，允许包含空格
    cout << "请输入任务日期（格式：xxxx-xx-xx）：" << endl;
    cin >> t.day;
    cout << "请输入开始时间（格式：xx:xx）：" << endl;
    cin >> t.start_time;
    cout << "请输入结束时间（格式：xx:xx）：" << endl;
    cin >> t.end_time;
    return cin;
}

// 添加或删除任务的功能函数
void addorremove()
{
    cout<<"若要添加请按1，删除请按2。"<<endl;
    int a;
    cin>>a;
    cout<<endl;
    
    if(a==1)
    {
        cout<<"课程请按1 / 考试请按2 / 作业请按3"<<endl;
        a=0;
        cin>>a;
        
        // 创建相应类型的任务对象
        Task* t = nullptr;
        if (a == 1) t = new CourseTask;           
        else if (a == 2) t = new ExamTask;
        else if (a == 3) t = new HomeworkTask;
        
        if (t) {                                  
            cin >> *t;   // 输入任务基本信息
            
            // 根据任务类型输入额外信息
            if (a == 1) {
                auto ct = dynamic_cast<CourseTask*>(t);//将基类指针 t 转换为派生类 CourseTask* 指针，通过派生类指针访问 classroom 成员变量
                cout << "请输入所在教室：" << endl;
                string c;  
                cin >> c;
                ct->classroom = c;//将用户输入的教室信息（临时变量 c）赋值给课程任务对象的 classroom 成员变量，通过指针 ct 访问对象成员
            }
            else if (a == 2) {
                auto et = dynamic_cast<ExamTask*>(t);
                cout << "请输入考场：" << endl;
                string c;
                cin >> c;
                et->classroom = c;
            }
            else if (a == 3) {
                auto ht = dynamic_cast<HomeworkTask*>(t);
                cout << "请输入截止时间（格式：xx:xx）：" << endl;
                string d;
                cin >> d;
                ht->deadline = d;
            }
            
            // 检查新任务是否与现有任务冲突
            for(Task*&l:list){         //遍历全局任务列表 list
                if(IsConflict(t,l))    //检查新创建的任务 t 是否与现有任务冲突
                {
                    cout<<"与原本时间冲突！！！"<<endl;
                    delete t;  // 冲突则删除临时创建的任务对象
                    return;
                }	
            }      
            
            // 添加任务到列表
            list.push_back(t);    //将新创建的任务对象指针 t 添加到全局任务列表 list 中，使任务对象成为系统数据的一部分，可被后续操作访问
            cout<<"添加成功！"<<endl;
        }		
    }
    else if(a==2)
    {
        cout<<"请输入你要删除的任务的日期和名称："<<endl;
        string a,b;
        cin>>a>>b;
        
        for(int i=0;i<int(list.size());i++)      //遍历任务列表 list
        {
            if(list[i]->day==a&&list[i]->name==b)//删除所有日期为 a 且名称为 b 的任务
            {
                list.erase(list.begin()+i);      //从任务列表 list 中删除索引为 i 的元素
                cout<<"删除成功！"<<endl;
            }
        }
    }
    else 
        cout<<"wrong!"<<endl;
}

// 全局任务计数类对象
TaskCounter counter;
                    
// 按日期查询日程的功能函数
void showschedule()
{
    cout<<"请输入你想要查询日程的日期（按照xxxx-xx-xx 年-月-日的形式）："<<endl;
    string a;
    cin>>a;
    counter.searchday(a);//通过遍历任务列表并比较日期字符串，实现了按日期搜索任务的功能
}	                     

// 统计当月任务数量的功能函数
void number()
{
    cout<<"请输入你想要查询任务数量的年份：";
    int a;
    cin>>a;
    cout<<endl;
    cout<<"请输入你想要查询任务数量的月份：";
    int b;
    cin>>b;
    cout<<endl;
    TaskCounter::report(a,b);//输入年份 a 和月份 b，统计该月份的课程任务、考试任务和作业任务数量，显示统计结果
}

// 按关键字搜索任务的功能函数
void searchimportant()
{
    cout<<"请输入你所要查找的关键字:"<<endl;
    string a;
    cin>>a;
    
    // 遍历所有任务，查找名称中包含关键字的任务
    for(Task*t:list)
    {
        if(t->name.find(a)!= string::npos)//查看是否存在位置
        {
            t->show();//根据 t 指向的实际对象类型，调用相应的 show() 实现
        }
    }
    cout<<"搜索结束 ------------"<<endl;
}

// 辅助函数：将时间字符串(HH:MM)转换为分钟数
int stoint(string time)
{
    string ti1=time.substr(0,2);  // 提取小时部分
    string ti2=time.substr(3,2);  // 提取分钟部分
    int hour=stoi(ti1);           // 转换为整数
    int min=stoi(ti2);            // 转换为整数
    return 60*hour+min;           // 返回总分钟数
}

// 判断两个任务是否时间冲突
bool IsConflict(Task*t1,Task*t2)
{
    if(t1->day!=t2->day)
        return false;  // 不同日期的任务不冲突
    
    // 判断时间是否重叠
    if(stoint(t1->end_time)<=stoint(t2->start_time))
    {
        return false;  
    }
    else if(stoint(t1->start_time)>=stoint(t2->end_time))
    {
        return false;  
    }
    
    return true;  // 否则冲突
}

// 系统菜单函数
void menu() {
    cout << "  智能日程管理系统   " << endl;
    
    int a;
    while (true) {
        cout << "当前共存任务：" << Task::num << " 条"<<endl;//num没设置为私有成员的原因
        cout << "智能日程管理系统菜单" << endl;
        cout << "1. 添加/删除任务" << endl;
        cout << "2. 查询日程" << endl;
        cout << "3. 统计当月任务数量" << endl;
        cout << "4. 搜素关键词" << endl;
        cout << "5. 退出系统" << endl;
        cout << "请选择操作编号：";
        cin >> a;
        
        switch (a) {
            case 1:
                addorremove();
                break;
            case 2:
                showschedule();
                break;
            case 3:
                number();
                break;
            case 4:	
                searchimportant();
                break;	
            case 5:	
                cout << "感谢使用，再见！" << endl;
                return;
            default:
                cout << "无效选择，请重新输入" << endl;
        }
    }
}
//主函数
int main()
{   // 初始化测试数据
    list.push_back(new CourseTask("高等数学", "2025-06-13", "09:00", "10:30", "三教A112"));//使用 new 分配对象
    list.push_back(new CourseTask("大学英语", "2025-06-10", "15:45", "18:15", "一教205"));
    list.push_back(new ExamTask("英语期中", "2025-06-11", "10:00", "11:30", "A102"));
    list.push_back(new HomeworkTask("课程设计", "2025-06-10", "00:00", "23:59", "23:59"));
    list.push_back(new CourseTask("大学物理", "2025-06-06", "08:00", "09:30", "三教A212"));
    
    // 显示菜单并处理用户操作
    menu();
    
    // 释放内存，避免内存泄漏
    for (Task* t : list) {     //for自动遍历 list 中的每个元素,若 Task 析构函数为虚函数，会正确调用派生类析构函数
        delete t;              //调用 t 指向对象的析构函数
    }
    list.clear();              //调用 clear() 方法清空任务列表
    return 0;
}
