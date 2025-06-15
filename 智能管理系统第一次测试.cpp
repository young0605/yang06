#include<iostream>
#include<string>
#include<vector>//��̬����
#include<algorithm>//�ṩsort����
using namespace std;
class Task;
vector<Task*> list;//����洢�����������������
// ������������ʱ���ַ���(HH:MM)ת��Ϊ������
int stoint(string time);
//������� - �����࣬���о����������ͼ̳��Դ���
class Task
{
public:
    // ��¼��ǰϵͳ�����������
    
    // ���캯������ʼ�����������Ϣ
    Task(string n,string d,string s,string e):name(n),day(d),start_time(s),end_time(e){num++;}
    //������������ȷ���ȵ�����������������������ٵ��û�������������
    virtual ~Task(){num--;}//��ɾ������ʱ�������Զ���1��
    // ���麯����������ʾ������Ϣ�����������ʵ��
    virtual void show()=0;//ͨ�����麯��show()ʵ�ֶ�̬����ͬ���������в�ͬ����ʾ��ʽ
    
    // ��Ԫ��ͺ������������Task��˽�г�Ա
    friend class TaskCounter;
    friend struct comparebytime;
    friend bool IsConflict(Task*t1,Task*t2);
    friend istream& operator>>(istream& cin, Task& t);//��������أ�ͨ��cinֱ��д��name��day��start_time��end_time
    friend void addorremove();
    friend void searchimportant();
    static int num;	
protected:
    string name;      // ��������
    string day;       // ��������(��ʽ: YYYY-MM-DD)
    string start_time;// ��ʼʱ��(��ʽ: HH:MM)
    string end_time;  // ����ʱ��(��ʽ: HH:MM)
	
};

// ��ʼ����̬��Ա����
int Task::num=0;//��̬��Աͳ����������

//�γ������� ��������
class CourseTask:public Task{
public:
    string classroom;	
    
    // �޲ι��캯��
    CourseTask() : Task("", "", "", ""),classroom("") {} 
    // �������Ĺ��캯��
    CourseTask(string n,string d,string s,string e,string c):Task(n,d,s,e),classroom(c){}
    
    // ʵ�ֻ���Ĵ��麯������ʾ�γ�������Ϣ
    void show(){
        cout << "���γ̡�" << name <<"  ʱ��:"<<start_time<<'-'<<end_time<< "     ���ң�" << classroom << endl;
    }	//��дshow()��ʵ�ֲ��컯��������Ϣչʾ
};


 //���������� -��������
class ExamTask :public Task{
public:
    string classroom;	// ������Ϣ
    
    // �޲ι��캯��
    ExamTask():Task("", "", "", ""),classroom("") {} 
    // �������Ĺ��캯��
    ExamTask(string n,string d,string s,string e,string c):Task(n,d,s,e),classroom(c){}
    
    // ʵ�ֻ���Ĵ��麯������ʾ����������Ϣ
    void show(){
        cout << "�����ԡ�" << name  <<"  ʱ��:"<<start_time<<'-'<<end_time<< "     ������" << classroom << endl;
    }	
};

 // ��ҵ������ -�������� 
class HomeworkTask :public Task{
public:
    string deadline;	// ��ֹʱ��
    
    // Ĭ�Ϲ��캯��
    HomeworkTask():Task("", "", "", ""),deadline("") {} 
    // �������Ĺ��캯��
    HomeworkTask(string n,string d,string s,string e,string de):Task(n,d,s,e),deadline(de){}
    
    // ʵ�ֻ���Ĵ��麯������ʾ��ҵ������Ϣ
    void show(){
        cout << "���γ���ҵ��" << name  <<"  ʱ��:"<<start_time<<'-'<<end_time<<"     ��ֹʱ�䣺" << deadline << endl;
    }
};

// �Ƚ����ṹ�� - ���ڰ���ʼʱ��������������
struct comparebytime{                            
    bool operator()(Task*&a1,Task*&a2)const{     //bool�������ڴ洢�߼�ֵ���������ж�
        // ��ʱ���ַ���ת��Ϊ���������бȽ�
        return stoint(a1->start_time) < stoint(a2->start_time);//��ʱ���ַ���ת��Ϊ���������Ƚϣ��� a1 �Ŀ�ʼʱ������򷵻� true
    }
};

//�����ͳ�ƺͲ�ѯ��
class TaskCounter {
public:
    TaskCounter() {};//Ĭ�Ϲ����ʼ��
    ~TaskCounter() {};//����
    
    // ��������������
    void searchday(string cinday)//ö���ҳ�������cinday��ͬ������
    {
        ordertask.clear();     // �����һ����ѯ�����б�
       
        for(int i=0;i<int(list.size());i++)//����ȫ�������б� list���Դ洢�ģ���ɸѡ�����ڵ��� cinday ������
        {                                  
        
            if(list[i]->day==cinday)       //�ж��Ƿ����cinday
            {
                ordertask.push_back(list[i]);/*ʹ�� push_back() ����������������ָ����ӵ� ordertask ordertask �洢 */
                
            }
        }
        //�Է��Ͻ�������
        orderastime();
    }
    
   // ��ʱ��������ʾ 
    void orderastime(){
        sort(ordertask.begin(), ordertask.end(), comparebytime());//�����Զ��������������
        for(Task*i:ordertask){                              
            i->show();                           //��̬��ʾ
        }
    }
    
    // ��̬��Ա������ͳ��ָ���µ���������
    static void report(int y,int m)
    {
        // ��ʽ�������ַ���
        string year=to_string(y);               //���ڽ��������͵��ꡢ��ת��Ϊ�ַ�����ʽ
        string month=to_string(m);
        if(m<10)
        {
            month="0"+month;                    
        }
        string ym=year+"-"+month;               //ʵ�ָ�ʽ����2025-06
        
        // ��ʼ��������������
        int countc= 0;
        int counte= 0;
        int counth= 0;
        
        // ��֤�����ʽ
        if (ym.size()!=7||ym[4]!='-') 
		{      
            cout << "ͳ��ʧ��!"<<endl;
            return;
        }
        
        // ������������ͳ��ָ�����µ���������
        for (Task*t:list)
        {
            if (t->day.substr(0,7)==ym)       /*substr(0, 7) ��ȡ�������ڴ�0��ʼ����Ϊ7���ִ�*/
            {
                // ʹ��dynamic_cast�ж���������
                if (dynamic_cast<CourseTask*>(t) != nullptr)      ++countc;
                else if (dynamic_cast<ExamTask*>(t) != nullptr)   ++counte;
                else if (dynamic_cast<HomeworkTask*>(t) != nullptr) ++counth;
            }
        }
        
        // ���ͳ�ƽ��
        cout << " �¶�����ͳ�� ����"<<endl;
        cout << "  �γ�����   ��" << countc<< "��"<<endl;
        cout << "  ��������   ��" << counte<< "��"<<endl;
        cout << "  ��ҵ����   ��" << counth<< "��"<<endl;
        cout << "������������������������������"<<endl;
    }
    
    vector<Task*>ordertask;//�洢����ָ��Ķ�̬���飬�洢ɸѡ��ķ��������������Ӽ�
};

// ��������������������������������Ϣ
istream& operator>>(istream& cin, Task& t) 
{
    cout << "�������������ƣ����԰����ո񣩣�" << endl;
    cin.ignore();               // ����֮ǰ�����뻺����
    getline(cin, t.name);       // ��ȡ��������������������ո�
    cout << "�������������ڣ���ʽ��xxxx-xx-xx����" << endl;
    cin >> t.day;
    cout << "�����뿪ʼʱ�䣨��ʽ��xx:xx����" << endl;
    cin >> t.start_time;
    cout << "���������ʱ�䣨��ʽ��xx:xx����" << endl;
    cin >> t.end_time;
    return cin;
}

// ��ӻ�ɾ������Ĺ��ܺ���
void addorremove()
{
    cout<<"��Ҫ����밴1��ɾ���밴2��"<<endl;
    int a;
    cin>>a;
    cout<<endl;
    
    if(a==1)
    {
        cout<<"�γ��밴1 / �����밴2 / ��ҵ�밴3"<<endl;
        a=0;
        cin>>a;
        
        // ������Ӧ���͵��������
        Task* t = nullptr;
        if (a == 1) t = new CourseTask;           
        else if (a == 2) t = new ExamTask;
        else if (a == 3) t = new HomeworkTask;
        
        if (t) {                                  
            cin >> *t;   // �������������Ϣ
            
            // ���������������������Ϣ
            if (a == 1) {
                auto ct = dynamic_cast<CourseTask*>(t);//������ָ�� t ת��Ϊ������ CourseTask* ָ�룬ͨ��������ָ����� classroom ��Ա����
                cout << "���������ڽ��ң�" << endl;
                string c;  
                cin >> c;
                ct->classroom = c;//���û�����Ľ�����Ϣ����ʱ���� c����ֵ���γ��������� classroom ��Ա������ͨ��ָ�� ct ���ʶ����Ա
            }
            else if (a == 2) {
                auto et = dynamic_cast<ExamTask*>(t);
                cout << "�����뿼����" << endl;
                string c;
                cin >> c;
                et->classroom = c;
            }
            else if (a == 3) {
                auto ht = dynamic_cast<HomeworkTask*>(t);
                cout << "�������ֹʱ�䣨��ʽ��xx:xx����" << endl;
                string d;
                cin >> d;
                ht->deadline = d;
            }
            
            // ����������Ƿ������������ͻ
            for(Task*&l:list){         //����ȫ�������б� list
                if(IsConflict(t,l))    //����´��������� t �Ƿ������������ͻ
                {
                    cout<<"��ԭ��ʱ���ͻ������"<<endl;
                    delete t;  // ��ͻ��ɾ����ʱ�������������
                    return;
                }	
            }      
            
            // ��������б�
            list.push_back(t);    //���´������������ָ�� t ��ӵ�ȫ�������б� list �У�ʹ��������Ϊϵͳ���ݵ�һ���֣��ɱ�������������
            cout<<"��ӳɹ���"<<endl;
        }		
    }
    else if(a==2)
    {
        cout<<"��������Ҫɾ������������ں����ƣ�"<<endl;
        string a,b;
        cin>>a>>b;
        
        for(int i=0;i<int(list.size());i++)      //���������б� list
        {
            if(list[i]->day==a&&list[i]->name==b)//ɾ����������Ϊ a ������Ϊ b ������
            {
                list.erase(list.begin()+i);      //�������б� list ��ɾ������Ϊ i ��Ԫ��
                cout<<"ɾ���ɹ���"<<endl;
            }
        }
    }
    else 
        cout<<"wrong!"<<endl;
}

// ȫ��������������
TaskCounter counter;
                    
// �����ڲ�ѯ�ճ̵Ĺ��ܺ���
void showschedule()
{
    cout<<"����������Ҫ��ѯ�ճ̵����ڣ�����xxxx-xx-xx ��-��-�յ���ʽ����"<<endl;
    string a;
    cin>>a;
    counter.searchday(a);//ͨ�����������б��Ƚ������ַ�����ʵ���˰�������������Ĺ���
}	                     

// ͳ�Ƶ������������Ĺ��ܺ���
void number()
{
    cout<<"����������Ҫ��ѯ������������ݣ�";
    int a;
    cin>>a;
    cout<<endl;
    cout<<"����������Ҫ��ѯ�����������·ݣ�";
    int b;
    cin>>b;
    cout<<endl;
    TaskCounter::report(a,b);//������� a ���·� b��ͳ�Ƹ��·ݵĿγ����񡢿����������ҵ������������ʾͳ�ƽ��
}

// ���ؼ�����������Ĺ��ܺ���
void searchimportant()
{
    cout<<"����������Ҫ���ҵĹؼ���:"<<endl;
    string a;
    cin>>a;
    
    // �����������񣬲��������а����ؼ��ֵ�����
    for(Task*t:list)
    {
        if(t->name.find(a)!= string::npos)//�鿴�Ƿ����λ��
        {
            t->show();//���� t ָ���ʵ�ʶ������ͣ�������Ӧ�� show() ʵ��
        }
    }
    cout<<"�������� ------------"<<endl;
}

// ������������ʱ���ַ���(HH:MM)ת��Ϊ������
int stoint(string time)
{
    string ti1=time.substr(0,2);  // ��ȡСʱ����
    string ti2=time.substr(3,2);  // ��ȡ���Ӳ���
    int hour=stoi(ti1);           // ת��Ϊ����
    int min=stoi(ti2);            // ת��Ϊ����
    return 60*hour+min;           // �����ܷ�����
}

// �ж����������Ƿ�ʱ���ͻ
bool IsConflict(Task*t1,Task*t2)
{
    if(t1->day!=t2->day)
        return false;  // ��ͬ���ڵ����񲻳�ͻ
    
    // �ж�ʱ���Ƿ��ص�
    if(stoint(t1->end_time)<=stoint(t2->start_time))
    {
        return false;  
    }
    else if(stoint(t1->start_time)>=stoint(t2->end_time))
    {
        return false;  
    }
    
    return true;  // �����ͻ
}

// ϵͳ�˵�����
void menu() {
    cout << "  �����ճ̹���ϵͳ   " << endl;
    
    int a;
    while (true) {
        cout << "��ǰ��������" << Task::num << " ��"<<endl;//numû����Ϊ˽�г�Ա��ԭ��
        cout << "�����ճ̹���ϵͳ�˵�" << endl;
        cout << "1. ���/ɾ������" << endl;
        cout << "2. ��ѯ�ճ�" << endl;
        cout << "3. ͳ�Ƶ�����������" << endl;
        cout << "4. ���عؼ���" << endl;
        cout << "5. �˳�ϵͳ" << endl;
        cout << "��ѡ�������ţ�";
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
                cout << "��лʹ�ã��ټ���" << endl;
                return;
            default:
                cout << "��Чѡ������������" << endl;
        }
    }
}
//������
int main()
{   // ��ʼ����������
    list.push_back(new CourseTask("�ߵ���ѧ", "2025-06-13", "09:00", "10:30", "����A112"));//ʹ�� new �������
    list.push_back(new CourseTask("��ѧӢ��", "2025-06-10", "15:45", "18:15", "һ��205"));
    list.push_back(new ExamTask("Ӣ������", "2025-06-11", "10:00", "11:30", "A102"));
    list.push_back(new HomeworkTask("�γ����", "2025-06-10", "00:00", "23:59", "23:59"));
    list.push_back(new CourseTask("��ѧ����", "2025-06-06", "08:00", "09:30", "����A212"));
    
    // ��ʾ�˵��������û�����
    menu();
    
    // �ͷ��ڴ棬�����ڴ�й©
    for (Task* t : list) {     //for�Զ����� list �е�ÿ��Ԫ��,�� Task ��������Ϊ�麯��������ȷ������������������
        delete t;              //���� t ָ��������������
    }
    list.clear();              //���� clear() ������������б�
    return 0;
}
