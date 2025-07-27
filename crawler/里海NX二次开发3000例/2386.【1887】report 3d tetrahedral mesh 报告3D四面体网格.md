### 【1887】report 3d tetrahedral mesh 报告3D四面体网格

#### 代码

```cpp
    /*HEAD REPORT_3D_TETRAHEDRAL_MESH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog在V18版本中是新增的。 */  
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
    /* qq3123197280 */  
    static int mask_for_meshes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_sfem_mesh_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_mesh(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a mesh", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_meshes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            mesh;  
        UF_SF_ugs_tet_mesh_parms_t   
            mesh_parms;  
        char  
            buf[133];  
        while ((mesh = select_a_mesh("Select Tetrahedral Mesh")) != NULL_TAG)  
        {  
            if(!UF_CALL(UF_SF_ask_ugs_tet_mesh_parms(mesh, &mesh_parms)))  
            {  
                sprintf(buf, "midnodes: %d\n", mesh_parms.midnodes);  
                if (!UF_UI_open_listing_window())  
                    UF_UI_write_listing_window(buf);  
                sprintf(buf, "element size: %f\n", mesh_parms.element_size);  
                UF_UI_write_listing_window(buf);  
                sprintf(buf, "midnode_option: %d\n", mesh_parms.midnode_option);  
                UF_UI_write_listing_window(buf);  
                sprintf(buf, "max midnode jacobian: %d\n", mesh_parms.maximum_midnode_jacobian);  
                UF_UI_write_listing_window(buf);  
                sprintf(buf, "minimum_face_angle: %f\n", mesh_parms.minimum_face_angle);  
                UF_UI_write_listing_window(buf);  
                sprintf(buf, "surf_mesh_size_variation: %f\n", mesh_parms.surf_mesh_size_variation);  
                UF_UI_write_listing_window(buf);  
                sprintf(buf, "tet_mesh_size_variation: %d\n", mesh_parms.tet_mesh_size_variation);  
                UF_UI_write_listing_window(buf);  
            }  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发示例程序，其主要功能是选择一个四面体网格，并输出该网格的一些参数信息。以下是代码的主要功能介绍：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了一个选择网格的函数select_a_mesh，使用对话框让用户选择一个四面体网格。
> 3. 定义了主函数do_it，在其中循环调用select_a_mesh选择网格，并使用UF_SF_ask_ugs_tet_mesh_parms查询网格的参数，然后输出这些参数。
> 4. 定义了ufusr函数作为程序的入口，在其中初始化NX Open API，调用do_it函数，然后结束API。
> 5. 定义了ufusr_ask_unload函数，用于卸载用户自定义程序。
>
> 通过这个程序，用户可以方便地选择一个四面体网格，并查看其参数信息。
>
