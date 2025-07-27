### 【2078】report element tag of fem mesh 报告有限元网格元素标签

#### 代码

```cpp
    /*HEAD REPORT_ELEMENT_TAG_OF_FEM_MESH CCC UFUNC*/  
    /*As pre-requisite a FEM mesh needs to be present.  
    Also in V18.0.2.1 and earlier versions UF_SF_locate_element_by_id  
    has no validity check on element_ids. (Refer to PR-4408225)*/  
    #include <stdio.h>   
    #include <stdlib.h>   
    #include <string.h>   
    #include <uf.h>   
    #include <uf_disp.h>   
    #include <uf_sf.h>  
    #include <uf_modl.h>   
    #include <uf_part.h>   
    #include <uf_ui.h>   
    #include <uf_obj.h>   
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数。 */  
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
    static void report_element_tag()  
    {  
        int  
            i,  
            num_of_ids = 7, /*num_of_ids has to be equal or less than the amount  
                               of existing elements*/  
            element_ids[7]={1, 2, 3, 4, 5, 6, 7}, /*array size should correlate to  
                               num_of_ids*/  
            num_of_elements;  
        char   
            msg[300] = { "" };  
        tag_t  
            mesh_tag = NULL_TAG,  
            *element_tags_p;  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "number of ids: %d\n", num_of_ids);  
        UF_CALL(UF_UI_write_listing_window ( msg ));  
        UF_CALL(UF_SF_locate_element_by_id(mesh_tag, num_of_ids, element_ids,  
                 &num_of_elements, &element_tags_p));  
        for(i=0 ; i<num_of_ids; i++)  
        {  
            sprintf(msg, "element_id %d has element tag: %d\n", element_ids[i], element_tags_p[i]);  
            UF_CALL(UF_UI_write_listing_window ( msg ));   
        }  
        UF_free(element_tags_p);     
    }          
    static void do_it(void)   
    {   
        report_element_tag();  
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

> 这段代码是一个NX的二次开发示例，其主要功能是获取有限元网格中指定元素ID对应的元素标签。以下是代码的主要功能介绍：
>
> 1. 引入头文件：代码引入了NX提供的各种UF函数的头文件，以便调用NX提供的API。
> 2. 错误处理函数：定义了一个错误处理函数report_error，用于打印出函数调用失败时的错误信息。
> 3. 获取元素标签的函数：定义了一个函数report_element_tag，用于获取指定元素ID的元素标签。这个函数首先调用UF_SF_locate_element_by_id来获取元素标签，然后遍历元素ID数组，打印每个元素ID对应的元素标签。
> 4. 主函数：定义了一个主函数do_it，用于调用report_element_tag函数。
> 5. NX初始化和终止：在ufusr函数中，首先调用UF_initialize进行NX初始化，然后调用do_it执行主要逻辑，最后调用UF_terminate终止NX。
> 6. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于在卸载该二次开发程序时返回立即卸载的标志。
>
> 总的来说，这段代码通过调用NX提供的API，实现了根据元素ID获取对应元素标签的功能，并包含了错误处理和NX初始化/终止等辅助逻辑。
>
