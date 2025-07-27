### 【1611】map selected face of linked feature to corresponding source face 映射选定链接特征的面到相应的源面

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_wave.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
    #include <uf_disp.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* qq3123197280 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static UF_UNDO_mark_id_t set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
        return mark_id;  
    }  
    static void roll_back_and_delete_undo_mark(UF_UNDO_mark_id_t mark_id)  
    {  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face,  
            source_face;  
        uf_list_p_t  
            feats,  
            temp;  
        UF_UNDO_mark_id_t  
            temp_mark;  
        while ((face = select_a_face("Find link source")) != NULL_TAG)  
        {  
            if (UF_ASSEM_is_occurrence(face))  
            {  
                ECHO("Selected face is an occurrence\n");  
                ECHO("Please select a face of a linked feature\n");  
                continue;  
            }  
            UF_CALL(UF_MODL_ask_face_feats(face, &feats));  
            for (temp = feats; temp != NULL; temp = temp->next)  
            {  
                source_face = NULL_TAG;  
                temp_mark = set_undo_mark("Set Current Feature");  
                UF_CALL(UF_MODL_set_current_feature(temp->eid));  
                UF_WAVE_map_link_geom_to_source(temp->eid, face, TRUE, &source_face);  
                roll_back_and_delete_undo_mark(temp_mark);  
                if (source_face != NULL_TAG) break;  
            }  
            UF_CALL(UF_MODL_delete_list(&feats));  
            if (source_face == NULL_TAG)  
            {  
                UF_CALL(UF_DISP_set_highlight(face, 1));  
                uc1601("This face doesn't seem to be part of a linked feature", TRUE);  
                UF_CALL(UF_DISP_set_highlight(face, 0));  
                continue;  
            }  
            else  
            {  
                UF_CALL(UF_DISP_set_highlight(source_face, 1));  
                uc1601("Source face highlighted.", TRUE);  
                UF_CALL(UF_DISP_set_highlight(source_face, 0));  
            }  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 错误报告功能：通过宏定义UF_CALL来包裹UF函数调用，如果函数调用失败，则会调用report_error函数报告错误。
> 2. 选择面：通过自定义的select_a_face函数实现选择面的功能，并支持过滤只选择实体面。
> 3. 撤销操作：通过自定义的set_undo_mark和roll_back_and_delete_undo_mark函数实现撤销操作。
> 4. 查找链接源面：通过自定义的do_it函数实现查找选中面的链接源面，并高亮显示源面。
> 5. 提示信息：通过自定义的ECHO函数打印提示信息。
> 6. 程序入口：通过ufusr函数作为程序的入口，初始化和结束NX API，并调用do_it执行主要功能。
> 7. 立即卸载：通过ufusr_ask_unload函数实现立即卸载功能。
>
> 综上所述，这段代码通过NX提供的API实现了一个简单的交互式查找面链接源的功能。
>
