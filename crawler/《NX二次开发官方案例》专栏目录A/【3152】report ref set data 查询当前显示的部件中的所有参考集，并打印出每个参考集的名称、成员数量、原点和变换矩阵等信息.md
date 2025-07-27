### 【3152】report ref set data 查询当前显示的部件中的所有参考集，并打印出每个参考集的名称、成员数量、原点和变换矩阵等信息

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中的新功能。 */  
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
    static tag_t ask_next_ref_set(tag_t part, tag_t refset)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_reference_set_type,  
            &refset)) && (refset != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(refset, &type, &subtype));  
    #ifdef UF_reference_design_model_subtype  
            if ((subtype == UF_reference_design_subtype) ||  
                (subtype == UF_reference_design_model_subtype) ||  
                (subtype == UF_reference_design_lightweight_subtype))  
                return refset;  
    #else  
            if (subtype == UF_reference_design_subtype) return refset;  
    #endif  
        }  
        return NULL_TAG;  
    }  
    static void report_ref_set_data(tag_t ref_set)  
    {  
        char  
            msg[255],  
            ref_set_name[MAX_ENTITY_NAME_SIZE+1];  
        double  
            origin[3],  
            mx[9];  
        int  
            n=0;  
        tag_t  
            *members;  
        UF_CALL(UF_ASSEM_ask_ref_set_data(ref_set, ref_set_name, origin, mx, &n, &members));  
        if(n > 0)  
            UF_free(members);  
        sprintf(msg, "%s contains %d members\n at origin: %f, %f, %f\  
            \nwith matrix:\nX: %f,%f,%f,\nY: %f,%f,%f,\nZ: %f,%f,%f\n",  
            ref_set_name, n, origin[0], origin[1],origin[2], mx[0],\  
                mx[1], mx[2], mx[3], mx[4], mx[5], mx[6], mx[7], mx[8]);  
        if (!UF_UI_open_listing_window())  
        {  
            UF_UI_write_listing_window(msg);  
            UF_UI_write_listing_window("=============================================\n");  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            ref_set = NULL_TAG;  
        while ((ref_set = ask_next_ref_set(part, ref_set)) != NULL_TAG)  
            report_ref_set_data(ref_set);  
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

> 这是段NX的二次开发代码，其主要功能是遍历当前活动部件中的所有参考集，并输出每个参考集的名称、成员数量、原点坐标和变换矩阵等信息。
>
> 主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于在调用NX API函数出错时，输出错误信息。
> 2. 定义ask_next_ref_set函数，用于遍历部件中的所有参考集，并返回下一个设计参考集类型的参考集对象。
> 3. 定义report_ref_set_data函数，用于输出指定参考集的详细信息。
> 4. do_it函数负责遍历当前活动部件中的所有参考集，并输出每个参考集的信息。
> 5. ufusr是NX二次开发程序的入口函数，其中首先调用UF_initialize进行初始化，然后调用do_it执行主要功能，最后调用UF_terminate进行终止。
> 6. ufusr_ask_unload函数用于设置当用户退出NX时，立即卸载此二次开发程序。
>
> 总的来说，这段代码利用NX的API遍历并输出当前活动部件中的所有设计参考集的详细信息，以辅助用户进行设计参考集的管理。
>
