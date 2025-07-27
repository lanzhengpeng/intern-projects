### 【3159】intersect a body with a ray pre v16 使用射线与实体相交，并创建交点

#### 代码

```cpp
    /*HEAD INTERSECT_A_BODY_WITH_A_RAY_PRE_V16 CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数，用于打印系统日志信息。 */  
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
    static tag_t select_a_body(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            body,  
            view;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY;  
        mask.object_type = UF_solid_type;  
        mask.object_subtype = 0;  
        mask.solid_type = UF_UI_SEL_FEATURE_BODY;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &resp, &body, cp, &view));  
        if (resp == 4 || resp == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(body, FALSE));  
            return body;  
        }  
        else return NULL_TAG;  
    }  
    static logical specify_point(double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616("Specify point", ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static logical prompt_vector(char *prompt, double vec[3])  
    {  
        char  
            menu[3][16] = { "X", "Y", "Z" };  
        int  
            irc,  
            junk;  
        irc = uc1609(prompt, menu, 3, vec, &junk);  
        if (irc == 3 || irc == 4) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            irc,  
            n,  
            o_body[1],  
            one = 1;  
        tag_t  
            body[1],  
            pnt;  
        double  
            identity[9] = { 1,0,0, 0,1,0, 0,0,1 },  
            intersection[3],  
            normal[3],  
            point[3],  
            ray[3] = { 0, 0, 0 },  
            zero[3] = { 0, 0, 0};  
        while ((body[0] = select_a_body("Select body to intersect")) &&  
            specify_point(point) && prompt_vector("Enter projection ray", ray))  
        {  
            FTN(uf6545)(body, &one, identity, zero, &one, point, ray, &irc, &n,  
                intersection, normal, o_body);  
            UF_CALL(irc);  
            if (n) UF_CALL(UF_CURVE_create_point(intersection, &pnt));  
        }  
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

> 这段NX二次开发代码的主要功能是用于选择一个实体并创建一条射线，然后计算该射线与所选实体之间的交点。代码的流程如下：
>
> 1. 初始化：调用UF_initialize()来初始化NX环境。
> 2. 主体选择：使用select_a_body()函数来提示用户选择一个实体。该函数使用UF_UI库中的选择功能来实现。
> 3. 指定点：调用specify_point()函数让用户指定射线的起点。
> 4. 输入向量：通过prompt_vector()函数让用户输入射线的方向向量。
> 5. 射线计算：使用FTN(uf6545)函数计算射线与实体的交点，并获取交点坐标和法向量。其中，射线使用起点和方向向量表示。
> 6. 创建交点：如果计算得到交点，则使用UF_CURVE_create_point()函数在NX中创建一个点来表示交点。
> 7. 重复循环：以上步骤会重复执行，直到用户取消选择实体为止。
> 8. 终止：调用UF_terminate()来关闭NX环境。
> 9. 卸载提示：提供ufusr_ask_unload()函数来提示NX在卸载用户程序时立即卸载。
>
> 该代码通过交互式方式实现了射线与实体交点的计算和显示，为用户提供了一个实用的二次开发示例。
>
