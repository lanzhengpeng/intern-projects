### 【3053】visualize facets 可视化面

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_facet.h>  
    #include <uf_disp.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
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
    static tag_t ask_next_faceted_model(tag_t part, tag_t model)  
    {  
        while  
        (  
            !UF_CALL(UF_OBJ_cycle_objs_in_part(part,UF_faceted_model_type,&model))  
            && (model != NULL_TAG)  
        )  
            if (!UF_ASSEM_is_occurrence(model)) return model;  
        return NULL_TAG;  
    }  
    static void display_facet(tag_t model, int facet)  
    {  
        int  
            ii,  
            n;  
        double  
            normals[5][3],  
            verts[5][3];  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_FACET_ask_vertices_of_facet(model, facet, &n, verts));  
        UF_CALL(UF_FACET_ask_normals_of_facet(model, facet, &n, normals));  
        for (ii = 0; ii < (n - 1); ii++)  
        {  
            UF_CALL(UF_DISP_display_temporary_line(NULL_TAG,  
                UF_DISP_USE_WORK_VIEW, &verts[ii][0], &verts[ii+1][0], &props));  
            UF_DISP_conehead(UF_DISP_USE_WORK_VIEW, &verts[ii][0],   
                &normals[ii][0], 0);  
        }  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG,  
            UF_DISP_USE_WORK_VIEW, &verts[n-1][0], &verts[0][0], &props));  
        UF_DISP_conehead(UF_DISP_USE_WORK_VIEW, &verts[n-1][0],   
            &normals[n-1][0], 0);  
    }  
    static void do_it(void)  
    {  
        int  
            facet = UF_FACET_NULL_FACET_ID;  
        tag_t  
            model = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((model = ask_next_faceted_model(part, model)) != NULL_TAG)  
        {  
            while (!UF_CALL(UF_FACET_cycle_facets(model, &facet)) &&  
                (facet != UF_FACET_NULL_FACET_ID)) display_facet(model, facet);  
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

> 这段代码是一个NX Open的UF程序，其主要功能是在NX部件中循环遍历所有实体面片，并显示每个面片的顶点和法线。
>
> 主要步骤包括：
>
> 1. 初始化UF库。
> 2. 获取当前显示的部件。
> 3. 循环遍历部件中的实体面片模型，并逐个处理。
> 4. 对于每个面片模型，循环遍历所有面片，并逐个显示。
> 5. 显示面片时，获取面片的顶点和法线，并用线段连接顶点，并在顶点处显示法线箭头。
> 6. 继续处理下一个面片，直到所有面片都处理完毕。
> 7. 继续处理下一个面片模型，直到所有模型都处理完毕。
> 8. 终止UF库。
>
> 该代码使用UF函数实现了NX部件中面片模型的遍历和显示，可用于NX二次开发中的实体面片处理。
>
