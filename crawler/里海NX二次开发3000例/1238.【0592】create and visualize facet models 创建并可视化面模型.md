### 【0592】create and visualize facet models 创建并可视化面模型

#### 代码

```cpp
    /*HEAD CREATE_AND_VISUALIZE_FACET_MODELS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_facet.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据您的要求，UF_print_syslog是在V18版本中新增的。 */  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static void display_facet(tag_t model, int facet)  
    {  
        int  
            ii,  
            n;  
        double  
            verts[5][3];  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_FACET_ask_vertices_of_facet(model, facet, &n, verts));  
        for (ii = 0; ii < (n - 1); ii++)  
            UF_CALL(UF_DISP_display_temporary_line(NULL_TAG,  
                UF_DISP_USE_WORK_VIEW, &verts[ii][0], &verts[ii+1][0], &props));  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG,  
            UF_DISP_USE_WORK_VIEW, &verts[n-1][0], &verts[0][0], &props));  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            facet = UF_FACET_NULL_FACET_ID,  
            n;  
        tag_t  
            body = NULL_TAG,  
            model,  
            part = UF_PART_ask_display_part();  
        UF_FACET_parameters_t  
            parms;  
        char  
            msg[133];  
        UF_CALL(UF_FACET_ask_default_parameters(&parms));  
        parms.specify_curve_tolerance = TRUE;  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            while (prompt_for_a_number("Facet parameter", "Curve tolerance",  
                    &parms.curve_dist_tolerance))  
            {  
                UF_DISP_refresh();  
                UF_CALL(UF_FACET_facet_solid(body, &parms, &model));  
                n = 0;  
                while (!UF_CALL(UF_FACET_cycle_facets(model, &facet)) &&  
                    (facet != UF_FACET_NULL_FACET_ID))  
                {  
                    display_facet(model, facet);  
                    n++;  
                }  
                sprintf(msg, "Model has %d facets", n);  
                uc1601(msg, TRUE);  
            }  
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

> 这段代码是一个NX的二次开发示例，其主要功能是创建和可视化实体模型的facet网格。以下是代码的主要功能和关键点：
>
> 1. 错误处理：使用宏UF_CALL封装了NX的函数调用，并定义了report_error函数来打印错误信息。
> 2. 遍历实体：使用ask_next_body函数遍历指定部件中的所有实体。
> 3. facet网格参数：使用prompt_for_a_number函数提示用户输入facet网格参数，并显示参数改变后的facet网格。
> 4. 创建和显示facet网格：do_it函数遍历所有实体，并使用UF_FACET_facet_solid创建facet网格，然后使用display_facet函数显示网格。
> 5. NX初始化和终止：在ufusr函数中调用UF_initialize和UF_terminate初始化和终止NX。
> 6. 卸载提示：ufusr_ask_unload函数返回立即卸载标志。
>
> 总体来说，这段代码通过交互方式调整facet网格参数，并可视化显示facet网格，是一个典型的NX二次开发示例。
>
