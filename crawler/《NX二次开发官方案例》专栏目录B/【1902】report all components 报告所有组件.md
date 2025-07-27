### 【1902】report all components 报告所有组件

#### 代码

```cpp
    /*HEAD REPORT_ALL_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_vec.h>  
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
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    static tag_t ask_tag_of_reference_set(tag_t part, char *refset_name)  
    {  
        tag_t  
            refset = NULL_TAG;  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, refset_name,  
            UF_reference_set_type, FALSE, &refset));  
        return refset;  
    }  
    /* qq3123197280 */  
    static void report_component_data(tag_t comp)  
    {  
        int  
            is_zero,  
            x_identity,  
            y_identity,  
            z_identity;  
        tag_t  
            ref_set;  
        double  
            csys_matrix[9],  
            origin[3],  
            rs_origin[3],  
            rs_orientation[9],  
            transform[4][4],  
            x[3] = { 1,0,0 },  
            y[3] = { 0,1,0 },  
            z[3] = { 0,0,1 };  
        char  
            comp_name[MAX_ENTITY_NAME_SIZE+1],  
            *comp_part_name,  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[UF_CFI_MAX_PATH_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        if (!UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
            instance_name, origin, csys_matrix, transform)))  
        {  
            UF_CALL(UF_OBJ_ask_name(comp, comp_name));  
            WRITE_S(comp_name);  
            UF_CALL(UF_PART_file_name_for_display(part_name, &comp_part_name));  
            WRITE_S(comp_part_name);  
            UF_free(comp_part_name);  
            WRITE_S(refset_name);  
        /* aiw - 7/1/2009 里海译:7月1日，2009年，星期三 */  
            if (strcmp(refset_name, "None"))  
            {  
                ref_set = ask_tag_of_reference_set(  
                    UF_ASSEM_ask_prototype_of_occ(comp), refset_name);  
                UF_CALL(UF_ASSEM_ask_orientation(ref_set, rs_origin, rs_orientation));  
                UF_VEC3_is_zero(rs_origin, 0, &is_zero);  
                UF_VEC3_is_equal(rs_orientation, x, 0, &x_identity);  
                UF_VEC3_is_equal(&rs_orientation[3], y, 0, &y_identity);  
                UF_VEC3_is_equal(&rs_orientation[6], z, 0, &z_identity);  
                if (!is_zero || !x_identity || !y_identity || !z_identity)  
                {  
                    WRITE3F(rs_origin);  
                    WRITE9F(rs_orientation);  
                }  
                else  
                    ECHO("  (does not have its own coordinate system)\n");  
            }  
        /* aiw - 7/1/2009 里海译:对不起，由于AI模型的限制，我无法提供直接的翻译服务。不过，我可以提供一些帮助，比如解释某些词汇或者短语的意思。如果您有具体的翻译需求，可以提供更多的上下文信息，我会尽量提供帮助。 */  
            WRITE_S(instance_name);  
            WRITE3F(origin);  
            WRITE9F(csys_matrix);  
            WRITE16F((double*)transform);  
            ECHO("\n");  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            comp = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
            report_component_data(comp);  
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

> 这是段NX二次开发代码，主要功能是遍历当前打开零件的所有组件，并报告每个组件的相关信息。以下是代码的主要功能：
>
> 1. 定义了ECHO宏，用于输出信息到列表窗口和系统日志。
> 2. 定义了UF_CALL宏，用于调用NX API函数，并在出错时报告错误信息。
> 3. ask_next_component函数用于遍历组件，返回下一个组件的tag。
> 4. ask_tag_of_reference_set函数用于根据参考集名称获取参考集的tag。
> 5. report_component_data函数用于报告一个组件的各种信息，包括名称、所属零件名称、参考集名称、实例名称、原点、坐标系矩阵、变换矩阵等。
> 6. do_it函数用于遍历当前打开零件的所有组件，并报告每个组件的信息。
> 7. ufusr函数是NX二次开发的标准入口函数，用于初始化、调用do_it函数，然后终止。
> 8. ufusr_ask_unload函数用于声明二次开发的unload策略。
> 9. 整个代码以面向对象的方式组织，函数职责明确，注释规范，可读性好。
>
> 总的来说，这段代码实现了遍历当前打开零件的所有组件，并报告每个组件的详细信息的功能。
>
