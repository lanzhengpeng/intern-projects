### 【1509】list all reference sets 列出所有参考集

#### 代码

```cpp
    /*HEAD LIST_ALL_REFERENCE_SETS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static tag_t ask_next_ref_set(tag_t part, tag_t refset)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_reference_set_type,  
            &refset)) && (refset != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(refset, &type, &subtype));  
            if ((subtype == UF_reference_design_subtype) ||  
                (subtype == UF_reference_design_model_subtype) ||  
                (subtype == UF_reference_design_lightweight_subtype))  
                return refset;  
        }  
        return NULL_TAG;  
    }  
    #define SHOW_NAME(X) report_object_name(#X, X)  
    static void report_object_name(char *name, tag_t object)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN],  
            obj_name[MAX_ENTITY_NAME_SIZE + 1];  
        if (!UF_OBJ_ask_name(object, obj_name))  
            sprintf(msg, "%s (tag %d) is named %s\n", name, object, obj_name);  
        else  
            sprintf(msg, "%s (tag %d) is not named\n", name, object);  
        ECHO(msg);  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) ECHO(msg);  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                ECHO(msg);  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    ECHO(msg);  
                }  
            }  
            if (cols > 0) ECHO("\n");  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            ref_set = NULL_TAG;  
        double  
            origin[3],  
            orientation[9];  
        while ((ref_set = ask_next_ref_set(part, ref_set)) != NULL_TAG)  
        {  
            SHOW_NAME(ref_set);  
            UF_CALL(UF_ASSEM_ask_orientation(ref_set, origin, orientation));  
            WRITE3F(origin);  
            WRITE9F(orientation);  
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

> 这段代码是一个用于查询并打印NX模型中所有参考集的名称、原点和方向的二次开发代码。主要功能包括：
>
> 1. 定义了宏ECHO用于打印信息到列表窗口和系统日志。
> 2. 定义了宏UF_CALL用于调用NX API函数，并捕获错误代码。
> 3. 定义了报告错误函数report_error用于处理错误信息。
> 4. 定义了询问下一个参考集函数ask_next_ref_set用于查询模型中的所有参考集。
> 5. 定义了报告对象名称函数report_object_name用于打印对象名称。
> 6. 定义了写入数组到列表窗口的函数write_double_array_to_listing_window。
> 7. 定义了do_it函数，用于查询当前显示部件中的所有参考集，并打印名称、原点和方向。
> 8. 定义了ufusr函数作为主函数入口，初始化NX后调用do_it函数。
> 9. 定义了ufusr_ask_unload函数，用于卸载用户函数库。
>
> 综合来看，该代码实现了查询当前显示部件的所有参考集，并打印它们的名称、原点和方向的功能。代码结构清晰，使用宏和函数封装了重复代码，具有较好的可读性和可维护性。
>
