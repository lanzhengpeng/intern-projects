### 【2073】report drafting parameter differences 报告绘图参数差异

#### 代码

```cpp
    /*HEAD REPORT_DRAFTING_PARAMETER_DIFFERENCES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
    #define ECHOF(X)   (printf("%s = %f\n", #X, X))  
    #define ECHOS(X)   (printf("%s = %s\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意：UF_print_syslog是在V18版本中新增的，只需提供翻译，无需其他评论。 */  
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
    static int mask_for_draft_aids(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_draft_aid(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_draft_aids, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            mpi1[100],  
            mpi2[100];  
        tag_t  
            object1,  
            object2;  
        double  
            mpr1[70],  
            mpr2[70];  
        char  
            rad1[27],  
            rad2[27],  
            dia1[27],  
            dia2[27];  
        while (((object1 = select_a_draft_aid("Select 1st object to compare"))  
            != NULL_TAG) &&  
               ((object2 = select_a_draft_aid("Select 2nd object to compare"))  
            != NULL_TAG))  
        {  
            UF_CALL(UF_DRF_ask_object_preferences(object1, mpi1, mpr1, rad1, dia1));  
            UF_CALL(UF_DRF_ask_object_preferences(object2, mpi2, mpr2, rad2, dia2));  
            for (ii = 0; ii < 100; ii++)  
                if (mpi1[ii] != mpi2[ii])  
                {  
                    ECHO(ii);  
                    ECHO(mpi1[ii]);  
                    ECHO(mpi2[ii]);  
                }  
            for (ii = 0; ii < 70; ii++)  
                if (mpr1[ii] != mpr2[ii])  
                {  
                    ECHO(ii);  
                    ECHOF(mpr1[ii]);  
                    ECHOF(mpr2[ii]);  
                }  
            if (strcmp(rad1, rad2))  
            {  
                ECHOS(rad1);  
                ECHOS(rad2);  
            }  
            if (strcmp(dia1, dia2))  
            {  
                ECHOS(dia1);  
                ECHOS(dia2);  
            }  
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

```

#### 代码解析

> 这段NX二次开发代码的主要功能是比较两个制图辅助对象的参数差异。具体步骤如下：
>
> 1. 错误报告函数：定义了report_error函数，用于打印出函数调用失败时的错误代码和错误消息。
> 2. 制图辅助对象选择：定义了mask_for_draft_aids和select_a_draft_aid函数，用于选择两个制图辅助对象进行比较。
> 3. 参数比较：在do_it函数中，通过UF_DRF_ask_object_preferences函数获取两个对象的参数，包括整数参数数组mpi1/mpi2、浮点参数数组mpr1/mpr2、字符串参数rad1/rad2和dia1/dia2。然后逐个比较这些参数，如果有差异，则打印出来。
> 4. 初始化和结束：在ufusr函数中，首先初始化NX环境，然后调用do_it函数进行比较，最后终止NX环境。
> 5. 卸载请求：ufusr_ask_unload函数返回UF_UNLOAD_IMMEDIATELY，表示该程序可以被立即卸载。
>
> 总的来说，这段代码通过比较两个制图辅助对象的参数，帮助用户发现它们之间的差异，从而进行调试和修改。
>
