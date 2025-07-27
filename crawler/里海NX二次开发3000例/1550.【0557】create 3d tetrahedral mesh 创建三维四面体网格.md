### 【0557】create 3d tetrahedral mesh 创建三维四面体网格

#### 代码

```cpp
    /*HEAD CREATE_3D_TETRAHEDRAL_MESH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_sf_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 中新增的函数，用于打印系统日志信息。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    /* 里海 */  
    static int mask_for_cae_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_caegeom_type, UF_caegeom_body_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_cae_bodies(char *prompt, tag_t **cae_bodies)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Polygon Bodies", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_cae_bodies, NULL, &resp, &cnt, cae_bodies));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*cae_bodies)[ii], FALSE));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            *bodies,  
            mesh = NULL_TAG;  
        UF_SF_ugs_tet_mesh_parms_t   
            mesh_parms;  
        double  
            e_sz = 25.4;  
        char  
            buf[133];  
        int  
            n;  
        if (((n = select_cae_bodies("Select bodies to mesh", &bodies)) > 0)&&  
            prompt_for_a_number("Enter Element Size", "Element Size", &e_sz))         
        {  
            mesh_parms.midnodes = TRUE;  
            mesh_parms.element_size = 25.4;  
            mesh_parms.midnode_option = 2;  
            mesh_parms.maximum_midnode_jacobian = 0;  
            mesh_parms.minimum_face_angle = 20.0;  
            mesh_parms.surf_mesh_size_variation = 0;  
            mesh_parms.tet_mesh_size_variation = 20;  
            UF_CALL(UF_SF_create_ugs_tet_mesh(&mesh_parms, n, bodies, &mesh ));  
            if(mesh == NULL_TAG)  
            {  
                if (!UF_UI_open_listing_window())  
                    UF_UI_write_listing_window("Mesh not created, check parameters\n");  
            }  
            else  
            {  
                sprintf(buf, "New mesh tag: %d\n", mesh);  
                if (!UF_UI_open_listing_window())  
                    UF_UI_write_listing_window(buf);  
            }  
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

> 这段代码是用于在NX中进行3D四面体网格生成的二次开发代码。具体来说，该代码的主要功能包括：
>
> 1. 包含了必要的NX Open头文件，以调用NX的函数库。
> 2. 定义了一个错误报告函数report_error，用于在函数调用出错时报告错误。
> 3. 定义了一个提示用户输入数字的函数prompt_for_a_number。
> 4. 定义了一个用于选择CAE体的函数select_cae_bodies。
> 5. 定义了一个主函数do_it，用于选择要网格化的CAE体，提示用户输入网格单元尺寸，并调用NX函数生成四面体网格。
> 6. 定义了ufusr函数作为程序的入口，用于初始化NX Open环境，执行do_it函数，并结束NX Open环境。
> 7. 定义了ufusr_ask_unload函数，用于卸载NX Open库。
> 8. 整个程序流程为：选择要网格化的CAE体 -> 输入网格单元尺寸 -> 生成四面体网格。
> 9. 使用NX Open UI函数进行用户交互，使用NX Open CUF函数生成网格。
> 10. 具有错误处理机制，可以报告函数调用错误。
>
> 总的来说，这是一段比较完整的NX Open CUF二次开发程序，实现了在NX中交互选择几何体并生成四面体网格的功能。
>
