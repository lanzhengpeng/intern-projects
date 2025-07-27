### 【2331】report selected face type 报告选定面类型

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <stdarg.h>  
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
    static void do_it(void)  
    {  
        int  
            nai,  
            type;  
        double  
            na[6];  
        tag_t  
            face;  
        while ((face = select_a_face("Report face type")) != NULL_TAG)  
        {  
        //  These may return different values.  See PR 2156042.  
            UF_CALL(UF_MODL_ask_face_data(face, &type, na, na, na, na, na, &nai));  
            ECHO("UF_MODL_ask_face_data returned type = %d\n", type);  
            UF_CALL(UF_MODL_ask_face_type(face, &type));  
            ECHO("UF_MODL_ask_face_type returned type = %d\n", type);  
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

> 这段代码是用于NX软件的二次开发示例，主要功能是选择一个面并报告该面的类型。
>
> 主要步骤如下：
>
> 1. 引入必要的NX Open头文件。
> 2. 定义了一个ECHO函数，用于输出信息到日志窗口和系统日志。
> 3. 定义了一个UF_CALL宏，用于执行NX函数并检查返回码，如果出错则报告错误。
> 4. 定义了一个mask_for_faces函数，用于设置选择模式，只允许选择面特征。
> 5. 定义了一个select_a_face函数，用于使用对话框选择一个面，并返回面的tag。
> 6. 定义了一个do_it函数，用于循环选择面并报告面的类型。
> 7. ufusr函数是入口函数，初始化NX后调用do_it函数，最后终止NX。
> 8. ufusr_ask_unload函数用于卸载用户函数。
>
> 总的来说，这段代码通过交互式选择一个面，然后报告该面的类型，展示了NX二次开发的基本流程。
>
