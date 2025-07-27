### 【3107】evaluate faces 遍历当前显示的零件中的所有面，并计算每个面的曲面参数，包括位置、法向量、曲率等

#### 代码

```cpp
    /*HEAD EVALUATE_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
    #define ECHO2F(X)  (printf("  %s = %f, %f\n", #X, *X,*(X+1)))  
    #define ECHO3F(X)  (printf("    %s = %f, %f, %f\n", #X, *X,*(X+1),*(X+2)))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我理解UF_print_syslog是一个在V18版本中新增的功能，用于打印系统日志。 */  
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
    static tag_t ask_next_face(tag_t part, tag_t face)  
    {  
        int  
            subtype,  
            type;  
        while ((! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &face)))  
            && (face != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
            if (subtype == UF_solid_face_subtype) return face;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            mins[2],  
            uvs[4];  
        UF_MODL_SRF_VALUE_t  
            eval_result;  
        while ((face = ask_next_face(part, face)) != NULL_TAG)  
        {  
            ECHO(face);  
            UF_CALL(UF_DISP_set_highlight(face, TRUE));  
            UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
            mins[0] = uvs[0];  
            mins[1] = uvs[2];  
            ECHO2F(mins);  
            UF_CALL(UF_MODL_evaluate_face(face, UF_MODL_EVAL_ALL, mins,  
                &eval_result));  
            ECHO3F(eval_result.srf_pos);  
            ECHO3F(eval_result.srf_du);  
            ECHO3F(eval_result.srf_dv);  
            ECHO3F(eval_result.srf_unormal);  
            ECHO3F(eval_result.srf_d2u);  
            ECHO3F(eval_result.srf_dudv);  
            ECHO3F(eval_result.srf_d2v);  
            ECHO3F(eval_result.srf_d3u);  
            ECHO3F(eval_result.srf_d2udv);  
            ECHO3F(eval_result.srf_dud2v);  
            ECHO3F(eval_result.srf_d3v);  
            ECHO3F(eval_result.srf_normal);  
            UF_CALL(UF_DISP_set_highlight(face, FALSE));  
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

> 这段代码是用于NX的二次开发，其主要功能是遍历当前显示的零件中的所有面，并计算每个面的参数曲面的信息。具体来说，代码实现的功能包括：
>
> 1. 定义了宏ECHO、ECHO2F、ECHO3F，用于打印变量值。
> 2. 定义了宏UF_CALL，用于调用NX的函数，并处理错误。
> 3. 定义了函数report_error，用于打印错误信息。
> 4. 定义了函数ask_next_face，用于遍历零件中的面，并返回下一个面的tag。
> 5. 定义了函数do_it，遍历所有面，计算每个面的参数曲面的信息，并打印出来。
> 6. 定义了函数ufusr，用于初始化NX，调用do_it，然后终止NX。
> 7. 定义了函数ufusr_ask_unload，用于卸载用户自定义函数。
>
> 整体来说，这段代码实现了遍历零件中的面，并计算每个面的参数曲面的信息，将结果打印出来的功能。通过宏和函数的定义，代码结构清晰，易于理解和使用。
>
