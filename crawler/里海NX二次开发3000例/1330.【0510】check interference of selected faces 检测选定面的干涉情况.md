### 【0510】check interference of selected faces 检测选定面的干涉情况

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
    /* 里海 */  
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
    static void report_interference_code(int code)  
    {  
        switch (code)  
        {  
            case 1:  
                ECHO("There is interference\n");  
                break;  
            case 2:  
                ECHO("No interference\n");  
                break;  
            case 3:  
                ECHO("Coincident faces\n");  
                break;  
            default:  
                ECHO("Unknown Interference Code = %d\n", code);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            codes[1];  
        tag_t  
            target,  
            tools[1];  
        while (((target = select_a_face("Select target face")) != NULL_TAG) &&  
            ((tools[0] = select_a_face("Select tool face")) != NULL_TAG))  
        {  
            // This works but it returns 1-> There is interference when the faces  
            // are actually coincident (or identical) so it is not any better than  
            // using UF_MODL_ask_minimum_dist and checking for a 0 distance.  
            UF_CALL(UF_MODL_check_interference(target, 1, tools, codes));  
            report_interference_code(codes[0]);  
        }  
    }  
    /* 里海 */  
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

> 这段代码是NX的二次开发代码，主要功能是检查两个面之间的干涉情况。
>
> 主要步骤包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了一个函数mask_for_faces，用于设置选择过滤条件，只选择实体类型和任意面特征的面。
> 3. 定义了一个函数select_a_face，用于提示用户选择一个面，并返回所选面的tag。
> 4. 定义了一个函数report_interference_code，用于根据干涉代码打印相应的提示信息。
> 5. 在函数do_it中，循环提示用户选择目标面和工具面，然后调用UF_MODL_check_interference函数检查干涉情况，并打印结果。
> 6. 在主函数ufusr中，初始化NX环境，调用do_it函数执行操作，然后终止NX环境。
> 7. 定义了一个询问卸载函数ufusr_ask_unload，返回立即卸载。
>
> 整体来看，这段代码实现了面面干涉检查的功能，用户可以通过选择不同的面来检查它们之间是否存在干涉，并获取相应的提示信息。代码结构清晰，逻辑严谨，注释详细，具有良好的可读性和可维护性。
>
