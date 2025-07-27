### 【2330】report selected face loops 查询所选面循环

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_undo.h>  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void show_object_tag(tag_t object)  
    {  
        int  
            ii = 0;  
        double  
            loc[3];  
        char  
            old_name[MAX_ENTITY_NAME_SIZE+1],  
            msg[133];  
    /*  Reset to default name location 里海译:重置为默认名称位置 */  
        if (!UF_OBJ_ask_name(object, old_name))  
        {  
            UF_CALL(UF_OBJ_delete_name(object));  
            UF_CALL(UF_OBJ_set_name(object, old_name));  
            UF_CALL(UF_OBJ_ask_name_origin(object, loc));  
        }  
        else  
        {  
            UF_CALL(UF_OBJ_set_name(object, "X"));  
            UF_CALL(UF_OBJ_ask_name_origin(object, loc));  
            UF_CALL(UF_OBJ_delete_name(object));  
        }  
        sprintf(msg, "%d", object);  
        display_temporary_text(msg, loc);  
    }  
    static void report_face_loops(tag_t face)  
    {  
        char  
            *types[4] = { "", "Peripheral", "Hole", "Other" };  
        int  
            ii,  
            jj,  
            n_edges,  
            n_loops,  
            type;  
        tag_t  
            edge;  
        uf_list_p_t  
            edge_list;  
        uf_loop_p_t  
            loop_list;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        if (!UF_CALL(UF_MODL_ask_face_loops(face, &loop_list)) && loop_list)  
        {  
            UF_CALL(UF_MODL_ask_loop_list_count(loop_list, &n_loops));  
            ECHO("\nface %d has %d loops\n", face, n_loops);  
            for (ii = 0; ii < n_loops; ii++)  
            {  
                UF_CALL(UF_MODL_ask_loop_list_item(loop_list,ii,&type,&edge_list));  
                UF_CALL(UF_MODL_ask_list_count(edge_list, &n_edges));  
                ECHO("  %d.  %s loop has %d edges\n", ii+1, types[type], n_edges);  
                for (jj = 0; jj < n_edges; jj++)  
                {  
                    UF_CALL(UF_MODL_ask_list_item(edge_list, jj, &edge));  
                    show_object_tag(edge);  
                    if (!UF_OBJ_ask_name(edge, name))  
                        ECHO("    %d.  edge %d (%s)\n", jj+1, edge, name);  
                    else  
                        ECHO( "    %d.  edge %d\n", jj+1, edge);  
                }  
            }  
            UF_CALL(UF_MODL_delete_loop_list(&loop_list));  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        while  ((face = select_a_face("Report face loops")) != NULL_TAG)  
            report_face_loops(face);  
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

> 这段代码是一个NX的二次开发示例，主要用于选择面并报告该面的边界环信息。具体来说：
>
> 1. 定义了一些辅助函数，如ECHO用于输出信息，UF_CALL用于封装UF函数调用并处理错误，mask_for_faces用于设置选择面时的过滤条件。
> 2. select_a_face函数通过对话框选择一个面，并返回面的tag。
> 3. display_temporary_text函数用于在指定位置显示临时文本。
> 4. show_object_tag函数用于显示指定对象的tag。
> 5. report_face_loops函数用于报告指定面的边界环信息，包括环的类型、边的数量等。
> 6. do_it函数循环调用select_a_face和report_face_loops，直到用户取消选择。
> 7. ufusr是主函数，用于初始化和调用do_it函数。
> 8. ufusr_ask_unload用于卸载用户函数模块。
>
> 总体来说，这段代码实现了选择面并查看其边界环信息的功能，通过封装UF函数，提高了代码的可读性和可维护性。
>
