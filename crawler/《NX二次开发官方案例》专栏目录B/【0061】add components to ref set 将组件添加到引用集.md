### 【0061】add components to ref set 将组件添加到引用集

#### 代码

```cpp
    /*HEAD ADD_COMPONENTS_TO_REF_SET CCC UFUN */  
    /* This sample program will add all  
       of the components of the work part  
       to a chosen reference set of the  
       work part. The work part should NOT  
       be the same as the displayed part   */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 中新增的函数。

该函数用于打印系统日志信息，可以输出日志到控制台或文件。在 V18 之前版本中，需要使用其他方法来实现类似功能，而在 V18 版本中可以直接使用该函数来打印系统日志。 */  
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
    static void do_it(void)  
    {  
        char    name[38], ref_set_names[3][38];  
        tag_t   comp,  
                ref_set_array[14],  
                ref_set = NULL_TAG;  
        int     count=0, resp, cntr=0, ii;  
        tag_t   *children;  
        while( (ref_set_array[cntr] = ask_next_ref_set( UF_ASSEM_ask_work_part(),  
                ref_set)) && (cntr < 14))  
        {  
            printf("    Ref set tag: %d", ref_set_array[cntr]);  
            UF_CALL( UF_OBJ_ask_name( ref_set_array[cntr], name));  
            strcpy( ref_set_names[cntr], name);  
            printf(", name: %s\n", name);  
            ref_set = ref_set_array[cntr];  
            cntr++;  
        }  
        if(cntr > 0)  
        {  
            resp = uc1603("Select ref set", 0, ref_set_names, cntr);  
                printf("Response from uc1603: %d\n", resp);  
            if( (resp > 4) && (resp < 19) )  
            {  
                comp = UF_ASSEM_ask_work_occurrence();  
                count = UF_ASSEM_ask_part_occ_children( comp, &children);  
                    printf("Component count: %d\n", count);  
                for(ii = 0; ii < count; ii++)  
                    printf("  Component tag #%d) %d\n", ii+1, children[ii]);  
                    printf("    Adding to ref set tag: %d\n", ref_set_array[resp-5]);  
                UF_CALL( UF_ASSEM_add_ref_set_members( ref_set_array[resp-5], count, children));  
                UF_free( children);  
            }  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        if( UF_PART_ask_display_part() != UF_ASSEM_ask_work_part())  
            do_it();  
        else  
            uc1601("Work part must not be the displayed part", 1);  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是段NX的二次开发代码，主要功能是将工作部件的所有组件添加到用户选择的参考集。以下是代码的主要功能和流程：
>
> 1. 定义了错误报告函数report_error，用于打印出函数调用失败的错误信息。
> 2. 定义了ask_next_ref_set函数，用于遍历工作部件中的所有参考集，并返回下一个设计参考集。
> 3. 主函数do_it负责获取工作部件的所有设计参考集，并让用户选择一个参考集。然后获取工作部件的所有组件，并将这些组件添加到用户选择的参考集中。
> 4. ufusr是NX二次开发程序的入口函数。它首先初始化NX，然后判断当前显示的部件是否为工作部件，如果是则执行添加组件到参考集的操作，否则给出提示。最后终止NX。
> 5. ufusr_ask_unload函数用于处理二次开发程序的卸载，这里设置为立即卸载。
>
> 整体来看，这段代码实现了NX二次开发中添加组件到参考集的功能，并具有错误处理和用户交互的功能。
>
