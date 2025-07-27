### 【2487】report split faces 报告分割面

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_split_faces(tag_t face)  
    {  
        int  
            ii,  
            nfaces;  
        tag_t  
            *faces;  
        UF_CALL(UF_MODL_ask_split_faces(face, &nfaces, &faces));  
        if (nfaces > 1)  
        {  
            for (ii = 0; ii < nfaces; ii++ )  
                UF_CALL(UF_DISP_set_highlight(faces[ii], TRUE));  
            uc1601("Highlighted faces were once one face", TRUE);  
            for (ii = 0; ii < nfaces; ii++ )  
                UF_CALL(UF_DISP_set_highlight(faces[ii], FALSE));  
        }  
        else  
            uc1601("That face has not been split", 1);  
        if (nfaces > 0) UF_free(faces);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        while ((face = select_a_face("Select a face")) != NULL_TAG)  
            report_split_faces(face);  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 错误报告机制：定义了ECHO函数用于输出错误信息，以及UF_CALL宏用于调用NX API，并在出错时报告错误。
> 2. 选择面：定义了mask_for_faces函数，用于设置选择模式为只选择面，并实现了select_a_face函数，用于弹出选择对话框，让用户选择一个面。
> 3. 查询分割面：定义了report_split_faces函数，用于查询一个面是否被分割，如果被分割，则高亮显示所有分割后的面。
> 4. 主函数：定义了do_it函数，循环调用select_a_face获取用户选择的每一个面，然后调用report_split_faces查询并高亮显示分割后的面。
> 5. UFusr函数：这是NX二次开发程序的入口函数，用于初始化NX，调用do_it执行主逻辑，并在结束时终止NX。
> 6. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载标志，用于在二次开发程序卸载时调用。
>
> 整体来说，这段代码实现了一个简单的交互式NX二次开发程序，用于查询和显示面的分割情况。
>
