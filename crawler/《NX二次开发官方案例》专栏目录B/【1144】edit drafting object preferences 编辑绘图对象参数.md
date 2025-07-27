### 【1144】edit drafting object preferences 编辑绘图对象参数

#### 代码

```cpp
    /*HEAD EDIT_DRAFTING_OBJECT_PREFERENCES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意：UF_print_syslog是V18版本新增的功能，只提供翻译，不要添加其他无关内容。 */  
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
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            mpi[100];  
        tag_t  
            object;  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27];  
        while ((object=select_a_draft_aid("Select object to modify")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_object_preferences(object, mpi, mpr, rad, dia));  
            if (prompt_for_a_number("Enter new", "Aspect Ratio", &mpr[45]))  
                UF_CALL(UF_DRF_set_object_preferences(object, mpi, mpr, rad, dia));  
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

> 这段NX二次开发代码的主要功能是修改绘图辅助对象（如圆弧、文本等）的显示参数。
>
> 主要步骤包括：
>
> 1. 初始化UF模块。
> 2. 使用UF_UI模块提供的函数，让用户选择要修改的绘图辅助对象。
> 3. 获取选定对象的当前显示参数，包括线宽、颜色、线型等。
> 4. 提示用户输入新的显示参数，如线宽、颜色等。
> 5. 更新选定对象的显示参数。
> 6. 循环上述过程，直到用户不选择任何对象。
> 7. 退出UF模块。
>
> 关键函数包括：
>
> 总的来说，这段代码通过UI交互和绘图模块API，实现了修改绘图辅助对象显示参数的功能。
>
