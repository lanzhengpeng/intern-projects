### 【0785】create new part 创建新零件

#### 代码

```cpp
    /*HEAD CREATE_NEW_PART.CXX CCC UFUN */  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_part.hxx>  
    #include <ug_info_window.hxx>  
    static void do_it()  
    {  
        UgPart *part = UgPart::create("/users/webstera/1280417.prt", Inches);  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        try  
        {  
            UgSession my_session(true);  
            do_it();  
        }  
        catch (UgException oops)  
        {  
            UgInfoWindow  info;  
            info.open();  
            info.write(oops.askErrorText());  
            info.write("\n");  
            return;  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++的一个二次开发示例，主要功能是创建一个新的NX部件。
>
> 关键点如下：
>
> 1. 引入了UG NX二次开发需要的一些头文件，如ug_exception.hxx、ug_session.hxx、ug_part.hxx、ug_info_window.hxx等。
> 2. 定义了一个静态函数do_it()，该函数用于创建一个新的NX部件。
> 3. 在do_it()函数中，使用UgPart::create()方法创建了一个新的部件，部件的名称为"/users/webstera/1280417.prt"，单位为英寸。
> 4. 定义了一个ufusr()函数，这是NX二次开发中的标准入口函数。在ufusr()函数中，创建了一个UgSession对象，并调用do_it()函数以执行创建部件的操作。
> 5. 在ufusr()函数中，使用了try-catch异常处理，以捕获并处理可能抛出的UgException异常。
> 6. 如果捕获到异常，会打开一个UgInfoWindow信息窗口，显示异常信息，然后返回。
> 7. ufusr()函数的参数和返回值符合NX二次开发标准。
>
> 总的来说，这段代码提供了一个简单的示例，用于在NX中创建一个新的部件，并包含了基本的错误处理机制。
>
