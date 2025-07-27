### 【3105】demonstrate uf sf create fem 演示UF_SF_create_fem函数创建FEM的示例代码

#### 代码

```cpp
    /*HEAD DEMONSTRATE_UF_SF_CREATE_FEM CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_sf.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            cad_part_tag = UF_PART_ask_display_part(),  
            new_fem_tag = NULL_TAG;  
        const char  
            *description[2] = { "this is a test FEM part",  
                                "that was created by a GTAC application" };  
        if (!UF_CALL(UF_SF_create_fem ("gtac_example", cad_part_tag, NULL,  
            TRUE, 0, NULL, "NX NASTRAN", "Structural", 2, description,  
            &new_fem_tag ))) WRITE_D(new_fem_tag);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++应用程序，其主要功能是创建一个有限元模型(FEM)。
>
> 代码的关键步骤包括：
>
> 1. 包含必要的头文件，如uf.h, uf_sf.h等。
> 2. 定义宏ECHO和UF_CALL，用于打印信息和错误报告。
> 3. 定义静态函数report_error，用于打印错误信息。
> 4. 定义静态函数write_integer_to_listing_window，用于打印整数信息。
> 5. 定义静态函数do_it，用于创建FEM模型。
> 6. 在do_it中，首先获取当前显示的CAD部件，然后调用UF_SF_create_fem函数创建一个新的FEM模型。函数参数包括FEM名称、CAD部件、网格密度、输出类型、描述信息等。
> 7. 如果创建成功，打印新FEM的tag。
> 8. ufusr函数是程序的入口，首先初始化UF，然后调用do_it，最后终止UF。
> 9. ufusr_ask_unload函数用于卸载应用程序。
>
> 总体来说，这段代码实现了创建一个简单的FEM模型的功能，展示了NX二次开发的基本流程。
>
