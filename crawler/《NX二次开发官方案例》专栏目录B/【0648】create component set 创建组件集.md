### 【0648】create component set 创建组件集

#### 代码

```cpp
    /*HEAD CREATE_COMPONENT_SET CCC UFUN */  
    /*********  
    Sample program to create a cset (component set)  
    **********/  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog在V18版本中是新增的。 */  
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
    static int cycle_components  
        ( tag_t component,    /* Component to cycle           I/O 译:Component to cycle I/O */  
          tag_t cset          /* Component set to be modified I/O 译:组件设置待修改的I/O */  
        )  
    {  
        int inx,   
            ifail = 0,   
            component_count = 0;  
        tag_t *child_components = NULL;  
        logical level = FALSE;  
        if (component == NULL_TAG) return 0;  
        component_count = UF_ASSEM_ask_part_occ_children(component,  
                                                         &child_components);  
        for (inx = 0; inx < component_count; inx++)  
        {  
            UF_CALL(ifail = UF_ASSEM_add_to_cset(cset,   
                                                 child_components[inx],  
                                                 level));  
            if (ifail == 0)   
            {  
            ifail = cycle_components(child_components[inx], cset);  
            }  
            else break;  
        }  
        UF_free(child_components);  
        printf("component_count = %d\n", component_count);  
        return ifail;  
    }  
    static void do_it(void)  
    {  
     tag_t root_part_occ = NULL_TAG;  
     tag_t displayed_part = NULL_TAG;  
     tag_t cset = NULL_TAG;  
     static char* cset_name = "my_cset";  
     int rtcode = 0;  
     displayed_part = UF_PART_ask_display_part();  
     if (displayed_part != NULL_TAG)  
       {  
        root_part_occ = UF_ASSEM_ask_root_part_occ(displayed_part);  
        if (root_part_occ != NULL_TAG)   
            {      
              UF_CALL(UF_ASSEM_create_cset(displayed_part,  
                                             cset_name,   
                                             &cset));  
              rtcode = cycle_components(root_part_occ, cset);  
            }  
       }  
        if (rtcode == 0)  
        {  
            /*    
                This assumes we have successfully   
                executed the program.  
            */  
            printf("The UG/Open API program has succeeded.\n");  
            printf("Return code is %d\n", rtcode);  
        }  
        else  
        {  
            /*    
                This assumes we failed.  
            */  
            printf("The UG/Open API program has failed.\n");  
            printf("Return code is %d\n", rtcode);  
        }  
    }   
    /*****************************************************************/  
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

> 这段NX二次开发代码主要用于创建一个组件集（Component Set），其主要步骤如下：
>
> 1. 包含必要的头文件，如uf.h、uf_ui.h、uf_assem.h、uf_part.h等。
> 2. 定义了UF_CALL宏，用于调用NX API函数，并记录错误信息。
> 3. 定义了report_error函数，用于打印错误信息。
> 4. 定义了cycle_components函数，递归地将组件添加到组件集中。
> 5. 定义了do_it函数，用于获取当前显示的组件，创建组件集，并调用cycle_components函数填充组件集。
> 6. 定义了ufusr函数，用于初始化NX API、调用do_it函数，并终止NX API。
> 7. 定义了ufusr_ask_unload函数，用于卸载NX API。
>
> 这段代码的主要流程是：
>
> 1. 获取当前显示的组件。
> 2. 创建一个名为"my_cset"的组件集。
> 3. 递归地将组件及其子组件添加到组件集中。
> 4. 打印组件数量。
> 5. 判断操作是否成功，并打印相关信息。
>
> 总体而言，这段代码实现了创建组件集的功能，并通过递归调用将组件及其子组件添加到组件集中。
>
