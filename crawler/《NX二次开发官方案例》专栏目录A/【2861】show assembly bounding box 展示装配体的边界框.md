### 【2861】show assembly bounding box 展示装配体的边界框

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_fltr.h>  
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
    static void display_bounding_box( double llc[3], double urc[3])  
    {  
        double  
            box[6],  
            end[3],  
            start[3];  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_VEC3_copy(llc, box);  
        UF_VEC3_copy(urc, &(box[3]));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[1];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[1];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[1];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[1];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[4];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[4];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[4];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[4];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
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
    static void do_it(void)  
    {  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        double  
            centroid[3],  
            lower_left_corner[3],  
            offsets_to_corners[3],  
            orientation[9],  
            upper_right_corner[3];  
        UF_CALL(UF_FLTR_ask_box_of_assy(UF_ASSEM_ask_root_part_occ(part),  
            centroid, offsets_to_corners, orientation));  
        UF_VEC3_add(centroid, offsets_to_corners, upper_right_corner);  
        UF_VEC3_sub(centroid, offsets_to_corners, lower_left_corner);  
        display_bounding_box(lower_left_corner, upper_right_corner);  
        WRITE3F(centroid);  
        WRITE3F(offsets_to_corners);  
        WRITE9F(orientation);  
        WRITE3F(lower_left_corner);  
        WRITE3F(upper_right_corner);  
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

> 这段代码是一个NX Open API的二次开发示例，其主要功能如下：
>
> 1. 包含了常用的NX Open API头文件，如uf.h、uf_ui.h等，用于进行NX的二次开发。
> 2. 定义了ECHO宏，用于输出信息到NX的日志窗口。
> 3. 定义了UF_CALL宏，用于调用NX的API函数，并处理返回的错误码。
> 4. 定义了report_error函数，用于输出错误信息。
> 5. 定义了display_bounding_box函数，用于在NX中显示一个长方体的边界框。
> 6. 定义了WRITE3F和WRITE9F宏，用于将3x1和3x3的数组输出到日志窗口。
> 7. 定义了do_it函数，这是主函数，其流程如下：
>
> 1. 定义了ufusr函数，这是NX Open API的入口函数，初始化NX后调用do_it函数，最后终止NX。
> 2. 定义了ufusr_ask_unload函数，用于卸载用户函数时立即卸载。
>
> 总体来说，这段代码实现了在NX中获取装配体的位置信息，并显示其边界框的功能。通过阅读这段代码，可以学习到NX Open API的基本使用方法。
>
