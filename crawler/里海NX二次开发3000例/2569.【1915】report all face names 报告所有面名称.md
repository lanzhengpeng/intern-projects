### 【1915】report all face names 报告所有面名称

#### 代码

```cpp
    /*HEAD REPORT_ALL_FACE_NAMES.CPP CCC UFUN */  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_iterator.hxx>  
    #include <ug_face.hxx>  
    #include <ug_info_window.hxx>  
    #include <uf.h>  
    #include <uf_ui.h>  
    static void do_it(void)  
    {  
        UgInfoWindow info;  
        info.open();  
      // Construct an iterator for Unigraphics face objects  
         UgIterator < UgFace > curFace;  
      // Loop through all faces  
         while ( !curFace.isFinished ( ) )  
         {  
          // Get and ouput the name of the current face  
             std::string faceName = (*curFace)->getName ( );  
             if (faceName != "")  
             {  
                 info.write(faceName);  
                 info.write("\n");  
             }  
         //  Iterate to the next face  
             curFace.findNext ( );  
         }  
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
            oops.processError();  
        }  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++的二次开发示例，用于遍历当前打开的NX部件的所有面，并打印出它们的名称。
>
> 主要步骤如下：
>
> 1. 包含必要的NX Open头文件，如ug_exception.hxx、ug_session.hxx等。
> 2. 定义一个静态函数do_it()，用于遍历和打印面名称。
> 3. 在do_it()函数中，首先创建一个UgInfoWindow对象用于显示信息。
> 4. 构造一个UgIterator迭代器，用于遍历所有面。
> 5. 使用while循环遍历所有面，循环条件是迭代器未结束。
> 6. 在循环中，通过迭代器获取当前面的名称，如果名称不为空，就打印到UgInfoWindow中。
> 7. 每次循环结束后，调用findNext()方法，迭代到下一个面。
> 8. 定义ufusr()函数，这是NX二次开发的标准入口函数。
> 9. 在ufusr()函数中，创建UgSession对象并调用do_it()函数。
> 10. 在ufusr()函数中，使用try-catch捕获异常并处理。
> 11. 如果一切正常，这段代码会打开一个窗口，显示当前NX部件所有面的名称。
>
> 这个示例展示了如何使用NX Open C++ API遍历模型对象，并获取对象的基本属性。这对NX二次开发非常有用。
>
