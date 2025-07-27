### 【2098】report face periodicity 报告面周期性

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
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
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    static void report_face_periodicity(tag_t face)  
    {  
        int  
            U_status,  
            V_status;  
        double  
            U_period,  
            V_period;  
        UF_CALL(UF_MODL_ask_face_periodicity(face, &U_status, &U_period,  
            &V_status, &V_period));  
        WRITE_D(U_status);  
        WRITE_F(U_period);  
        WRITE_D(V_status);  
        WRITE_F(V_period);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        while ((face = select_a_face("Select face")) != NULL_TAG)  
            report_face_periodicity(face);  
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

> 这段代码是用于NX CAD软件的二次开发代码，其主要功能如下：
>
> 1. 包含了标准头文件，用于调用NX的UF模块接口。
> 2. 定义了一个宏UF_CALL，用于封装UF函数调用，并报告错误。
> 3. 定义了一个ECHO函数，用于在列表窗口和系统日志中输出信息。
> 4. 定义了一个report_error函数，用于输出UF函数调用的错误信息。
> 5. 定义了一个mask_for_faces函数，用于设置选择的面类型为实体面。
> 6. 定义了一个select_a_face函数，用于让用户选择一个面，并返回面标签。
> 7. 定义了两个宏WRITE_D和WRITE_F，用于输出整型和浮点型变量。
> 8. 定义了一个report_face_periodicity函数，用于查询一个面的周期性，并输出结果。
> 9. 定义了一个do_it函数，用于循环选择面并查询每个面的周期性。
> 10. 定义了ufusr函数，作为NX的入口函数，初始化NX环境，执行do_it函数，然后终止NX环境。
> 11. 定义了一个ufusr_ask_unload函数，用于卸载用户自定义函数。
>
> 整体来看，这段代码实现了在NX中选择一个面，并查询该面的周期性信息（是否是周期性边界）的功能。
>
