### 【1865】remove selected object from all reference sets 从所有引用集中移除选定对象

#### 代码

```cpp
    /*HEAD REMOVE_SELECTED_OBJECT_FROM_ALL_REFERENCE_SETS CCC UFUN */  
    /*  This program will move the selected items to the Unused Items group in  
        the Part Navigator.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数。 */  
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
    static tag_t select_an_object(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an object", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, NULL,NULL,&resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        logical  
            member;  
        int  
            ii,  
            n_ref_sets;  
        tag_t  
            object,  
            *ref_sets;  
        while ((object = select_an_object("Remove from all Reference sets"))  
            != NULL_TAG)  
        {  
            UF_CALL(UF_ASSEM_is_ref_set_member(object, &member));  
            if (!member)  
            {  
                uc1601("That object is not in any reference sets", TRUE);  
                continue;  
            }  
            UF_CALL(UF_ASSEM_ask_ref_sets(object, &n_ref_sets, &ref_sets));  
            for (ii = 0; ii < n_ref_sets; ii++)  
                UF_CALL(UF_ASSEM_remove_ref_set_members(ref_sets[ii], 1, &object));  
            UF_free(ref_sets);  
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

> 这段代码是用于NX CAD软件的二次开发，主要实现了以下功能：
>
> 1. 从所有引用集合中移除所选对象。
> 2. 使用UF函数来实现选择对象、检查是否在引用集合中、获取所有引用集合以及从引用集合中移除对象等功能。
> 3. 提供了一个错误处理函数，用于在UF函数调用失败时输出错误信息。
> 4. 主函数中通过循环调用选择对象函数，直到用户取消选择，然后检查对象是否在引用集合中，获取所有引用集合并从中移除该对象。
> 5. 最后，提供了卸载函数，用于在用户关闭NX时卸载该代码。
>
> 综上所述，该代码实现了从所有引用集合中移除所选对象的功能，并具有完善的错误处理机制，是NX二次开发的一个典型示例。
>
