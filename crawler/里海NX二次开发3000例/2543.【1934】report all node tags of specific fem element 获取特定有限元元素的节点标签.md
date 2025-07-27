### 【1934】report all node tags of specific fem element 获取特定有限元元素的节点标签

#### 代码

```cpp
    /*HEAD REPORT_ALL_NODE_TAGS_OF_SPECIFIC_FEM_ELEMENT CCC UFUNC*/  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的函数，主要用于打印系统日志信息。该函数为用户提供了打印系统日志的功能。 */  
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
    static void report_node_tag_of_fem_element_tag()  
    {  
        int  
            i,  
            num_of_ids = 1,   
            element_ids[1]={1},   
            num_of_elements,  
            num_of_nodes;  
        char   
            msg[300] = { "" };  
        tag_t  
            mesh_tag = NULL_TAG,  
            *element_tags_p,  
            *node_tags_p;  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_SF_locate_element_by_id(mesh_tag, num_of_ids, element_ids,  
                 &num_of_elements, &element_tags_p));  
        sprintf(msg, "element_id %d has element tag: %d\n", element_ids[0],   
                      element_tags_p[0]);  
        UF_CALL(UF_UI_write_listing_window ( msg ));   
        UF_CALL(UF_SF_locate_nodes_on_element(element_tags_p[0], &num_of_nodes,  
                &node_tags_p));     
        sprintf(msg, "number of nodes: %d\n", num_of_nodes);  
        UF_CALL(UF_UI_write_listing_window ( msg ));   
        for(i=0; i<num_of_nodes; i++)  
        {  
            sprintf(msg, "node_tag: %d\n", node_tags_p[i]);  
            UF_CALL(UF_UI_write_listing_window ( msg ));   
        }  
        UF_free(element_tags_p);  
        UF_free(node_tags_p);     
    }          
    static void do_it(void)   
    {   
        report_node_tag_of_fem_element_tag();  
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

> 这段代码是用于NX的二次开发，其主要功能是报告指定有限元元素的所有节点标签。
>
> 具体来说，代码首先定义了一个错误报告函数report_error，用于在调用UF函数出错时报告错误信息。
>
> 然后，定义了一个函数report_node_tag_of_fem_element_tag，该函数首先通过UF_SF_locate_element_by_id函数查找指定ID的有限元元素的标签，然后调用UF_SF_locate_nodes_on_element函数获取该元素的所有节点标签，并报告这些节点标签。
>
> 在主函数do_it中，调用了report_node_tag_of_fem_element_tag函数，从而实现了报告指定有限元元素的所有节点标签的功能。
>
> 最后，定义了ufusr函数作为NX的二次开发入口函数，在初始化后调用do_it函数，并在结束时进行终止。
>
> 总体来说，这段代码实现了对NX有限元模型指定元素的节点标签的查询和报告功能。
>
