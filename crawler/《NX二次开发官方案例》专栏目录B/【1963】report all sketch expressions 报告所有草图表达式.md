### 【1963】report all sketch expressions 报告所有草图表达式

#### 代码

```cpp
    /*HEAD REPORT_ALL_SKETCH_EXPRESSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_sket.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_sketch(tag_t part, tag_t sketch)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch));  
        return (sketch);  
    }  
    static void do_it(void)  
    {  
        int  
            num_expressions = 0,  
            inx = 0;  
        tag_t  
            sketch = NULL_TAG,  
            *expression_tags = NULL,  
            part = UF_PART_ask_display_part();  
        char  
            *exp_str = NULL;  
        UF_SKET_info_t   
            sketch_info;  
        UF_UI_exit_listing_window();  
        while((sketch=ask_next_sketch(part, sketch))!=NULL_TAG)  
        {  
            UF_CALL(UF_SKET_ask_sketch_info(sketch, &sketch_info ));  
            WRITE_S( sketch_info.name );  
            UF_CALL(UF_SKET_ask_exps_of_sketch(sketch, &num_expressions,   
                                                        &expression_tags));  
            WRITE_D( num_expressions );  
            for(inx = 0; inx < num_expressions; inx++)  
            {  
                WRITE_D( expression_tags[inx] );    
                UF_CALL( UF_MODL_ask_exp_tag_string(expression_tags[inx],   
                                                                 &exp_str));  
                WRITE( exp_str );  
                WRITE( "\n" );  
                UF_free(exp_str);  
            }  
            if(num_expressions > 0) UF_free( expression_tags );  
        }  
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

> 这段代码是一个NX的二次开发示例，主要用于报告NX部件中所有草图的名称和表达式信息。以下是主要代码结构和功能介绍：
>
> 1. 包含了必要的NX头文件，用于访问NX的模型、草图和表达式等对象。
> 2. 定义了宏函数UF_CALL，用于调用NX的UF函数，并在出错时报告错误。
> 3. 定义了report_error函数，用于在出错时输出错误信息。
> 4. 定义了WRITE系列宏函数，用于输出信息到日志窗口。
> 5. ask_next_sketch函数用于遍历部件中的所有草图，返回下一个草图的tag。
> 6. do_it函数是主要逻辑，遍历部件中的所有草图，并输出每个草图的名称和表达式信息。
> 7. ufusr函数是NX调用的主函数，初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. ufusr_ask_unload函数用于卸载NX环境。
>
> 通过该代码，可以快速获取和报告NX部件中所有草图的名称和表达式信息，对于草图信息的检查和报告非常有用。
>
