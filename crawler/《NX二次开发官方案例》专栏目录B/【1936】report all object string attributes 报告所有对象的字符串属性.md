### 【1936】report all object string attributes 报告所有对象的字符串属性

#### 代码

```cpp
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_typed.hxx>  
    #include <ug_part.hxx>  
    #include <ug_info_window.hxx>  
    #include <ug_attributable.hxx>  
    static void do_it(UgPart *part)  
    {  
        UgTypedObject  
            *typed_object;  
        UgInfoWindow  
            info;  
        info.open();  
        for ( typed_object = part->iterateFirst();  
              typed_object;  
              typed_object = part->iterateNext(typed_object) )  
        {  
            std::vector <std::string> titles =   
                typed_object->askStringAttributeTitles ( );  
            for (int ii = 0; ii < titles.size(); ii++)  
            {  
                if (ii == 0)  
                {  
                    std::string ObjectName = typed_object->getName();  
                    if ( ObjectName != "")  
                    {  
                        info.write(ObjectName);  
                        info.write(":\n");  
                    }  
                    else  
                    {  
                        info.write("Unnamed:\n");  
                    }  
                }  
                std::string value =   
                    typed_object->getStringAttributeValue(titles[ii]);  
                info.write("  ");  
                info.write(titles[ii]);  
                info.write(" = ");  
                info.write(value);  
                info.write("\n");  
            }  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        UgSession my_session(true);  
        try  
        {  
            UgPart *part = my_session.getWorkPart();  
            do_it(part);  
        }  
        catch (UgException oops)  
        {  
            oops.processError();  
        }  
    }  
    static int get_args ( int argc, char **argv, char *part_name )  
    {  
        UgInfoWindow  
            info;  
        if ( 2 != argc )  
        {  
            part_name [ 0 ] = '\0';  
            info.open();  
            info.write( "This program requires a part file name as input.\n");  
            info.write( "The syntax for this program is as follows:\n");  
            info.write( argv [ 0 ] );  
            info.write( " <part_name>\n");  
            return 1;  
        }  
        else if ( ( strlen ( argv [ 1 ] ) ) > MAX_FSPEC_SIZE )  
        {  
            info.open();  
            info.write("File name is too long.\n");  
            return 1;  
        }   
        else  
        {    
            strcpy ( part_name, argv [ 1 ] );  
            return 0;  
        }  
    }  
    extern int main ( int argc, char **argv )  
    {  
        try                                   // Begin error protection  
        {                                     
            int error;  
            char part_name [ MAX_FSPEC_SIZE + 1 ];  
            error = get_args ( argc, argv, part_name );  
            if ( ! error )  
            {  
                UgSession session ( true );   // Start a UG session  
                // Generate report  
                do_it ( UgPart::open ( part_name ) );   
                return 0;  
            }  
            else  
            {  
                return 1;  
            }  
        }  
        catch ( UgException &exception )      // Start of exception handler  
        {  
            exception.processError();  
            return 1;  
        }  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++应用程序，用于打开指定的NX部件文件，并遍历其中的对象，打印出对象的名称和属性值。
>
> 主要功能包括：
>
> 1. 处理输入参数：通过命令行参数获取要打开的部件文件名，并进行校验。
> 2. 打开NX部件文件：使用UgPart::open()函数打开指定的部件文件。
> 3. 遍历部件对象：使用iterateFirst()和iterateNext()方法遍历部件中的所有对象。
> 4. 获取对象属性：通过askStringAttributeTitles()和getStringAttributeValue()方法获取对象的属性名称和值。
> 5. 打印对象属性：将对象的名称和属性值打印到信息窗口。
> 6. 错误处理：使用try-catch语句处理程序运行过程中可能发生的异常。
> 7. 命令行主函数：定义了main()函数作为程序的入口点。
>
> 该程序通过遍历NX部件文件中的对象，打印出对象的属性信息，实现了对NX部件文件结构的探索和查看。
>
