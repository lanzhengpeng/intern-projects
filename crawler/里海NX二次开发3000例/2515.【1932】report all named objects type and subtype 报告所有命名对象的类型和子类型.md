### 【1932】report all named objects type and subtype 报告所有命名对象的类型和子类型

#### 代码

```cpp
    /*HEAD REPORT_ALL_NAMED_OBJECTS_TYPE_AND_SUBTYPE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_layer.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数，用于打印系统日志信息。该函数是V18版本特有的，因此需要注意版本兼容性。 */  
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
    static void do_it(void)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        while ( !UF_LAYER_cycle_by_layer(0, &object) && (object != NULL_TAG))  
            if ( !UF_OBJ_ask_name(object, name))  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
                printf("%s - %d/%d\n", name, type, subtype);  
            }  
    }  
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
    static void check_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    void main( int argc, char *argv[] )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            int  
                cnt = 0,  
                ii;  
            tag_t  
                part;  
            UF_PART_load_status_t  
                status;  
            for (ii=1; ii<argc; ii++)  
            {  
                printf("%d.  %s\n", ++cnt, argv[ii]);  
                if (UF_CALL(UF_PART_open(argv[ii], &part, &status))  
                    || status.n_parts > 0)  
                    check_load_status(&status);  
                else  
                {  
                    do_it();  
                    UF_CALL(UF_PART_save());  
                    UF_PART_close_all();  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，其主要功能是遍历当前打开的部件中的所有对象，并输出每个对象的名称、类型和子类型。以下是代码的详细说明：
>
> 1. 头文件：引入了标准C库、UF库、UF UI库、UF对象库和UF图层库的头文件。
> 2. 宏定义：定义了UF_CALL宏，用于调用UF函数并处理错误。
> 3. report_error函数：用于打印错误信息。
> 4. do_it函数：遍历所有图层，对每个图层中的对象调用UF_OBJ_ask_name获取名称，UF_OBJ_ask_type_and_subtype获取类型和子类型，并打印输出。
> 5. ufusr函数：NX二次开发程序的入口函数，初始化UF库，调用do_it函数，然后终止UF库。
> 6. ufusr_ask_unload函数：用于设置NX二次开发程序的卸载模式。
> 7. check_load_status函数：用于打印打开部件的状态信息。
> 8. main函数：程序入口，处理命令行参数，打开指定的部件，调用do_it函数，保存并关闭部件，最后打印处理过的部件数量。
>
> 总体来说，这段代码的主要目的是遍历并输出当前打开部件中所有对象的名称、类型和子类型，以帮助开发者了解部件的结构。
>
