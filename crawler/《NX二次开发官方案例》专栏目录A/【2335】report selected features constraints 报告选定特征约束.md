### 【2335】report selected features constraints 报告选定特征约束

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_FEATURES_CONSTRAINTS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #include <uf_mtx.h>  
    #include <uf_trns.h>  
    #include <uf_obj.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
    #define ECHOS(X)   (printf("%s = %s\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog 是在 V18 版本中新增的。 */  
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
    static int select_features(char *prompt, tag_t **feats)  
    {  
        int  
            cnt,  
            resp;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, feats, &resp));  
        if (resp == UF_UI_OK) return cnt;  
        else return 0;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static void map_plane2abs(double origin[3], double xyz[9], double vec[3])  
    {  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            irc;  
        UF_VEC3_copy(origin, csys);  
        UF_MTX3_copy(xyz, &csys[3]);  
        FTN(uf5940)(csys, abs, mx, &irc);  
        FTN(uf5941)(vec, mx);  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }  
    static void report_tag_type_and_subtype(char *name, tag_t object)  
    {  
        int  
            subtype,  
            type;  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            printf("%s = %d, type = %d, subtype = %d\n", name,object,type,subtype);  
        }  
        else printf("%s = %d (NULL_TAG)\n", name, object);  
    }  
    static void show_constraint_data(tag_t tool, tag_t part, char *how_far,  
        double fixed1[2], double fixed2[2], double feature1[2], double feature2[2],  
        double plane_org[3], double plane_xyz[9])  
    {  
        double  
            point[3];  
        report_tag_type_and_subtype("tool", tool);  
        report_tag_type_and_subtype("part", part);  
        ECHOS(how_far);  
        UF_free(how_far);  
        UF_VEC2_copy(fixed1, point);  
        point[2] = 0;  
        map_plane2abs(plane_org, plane_xyz, point);  
        display_temporary_point(point);  
        UF_VEC2_copy(fixed2, point);  
        point[2] = 0;  
        map_plane2abs(plane_org, plane_xyz, point);  
        display_temporary_point(point);  
        UF_VEC2_copy(feature1, point);  
        point[2] = 0;  
        map_plane2abs(plane_org, plane_xyz, point);  
        display_temporary_point(point);  
        UF_VEC2_copy(feature2, point);  
        point[2] = 0;  
        map_plane2abs(plane_org, plane_xyz, point);  
        display_temporary_point(point);  
    }  
    static void report_feature_constraints(tag_t feat)  
    {  
        int  
            ii,  
            n_constraints,  
            prllel;  
        tag_t  
            *constraints,  
            part,  
            tool;  
        uf_list_p_t  
            constraint_list;  
        char  
            *constraint_type,  
            *feat_name,  
            *how_far;  
        double  
            cplane_csys[12],  
            fixed1[2],  
            fixed2[2],  
            feature1[2],  
            feature2[2];  
        UF_CALL(UF_MODL_ask_feat_name(feat, &feat_name));  
        ECHOS(feat_name);  
        if (UF_CALL(UF_MODL_ask_constraints(feat, &constraint_list))) return;  
        n_constraints = make_an_array(&constraint_list, &constraints);  
        ECHO(n_constraints);  
        for (ii = 0; ii < n_constraints; ii++)  
        {  
            UF_CALL(UF_MODL_ask_constraint_type(constraints[ii], &constraint_type));  
            if (!strcmp(constraint_type, "LINK_FACE_PARMS"))  
            {  
                UF_CALL(UF_MODL_ask_link_face_plane(feat, constraints[ii],  
                    cplane_csys));  
                display_conehead(&cplane_csys[9], cplane_csys);  
                display_conehead(&cplane_csys[9], &cplane_csys[3]);  
                display_conehead(&cplane_csys[9], &cplane_csys[6]);  
                break;  
            }  
        }  
        for (ii = 0; ii < n_constraints; ii++)  
        {  
            UF_CALL(UF_MODL_ask_constraint_type(constraints[ii], &constraint_type));  
            printf("  %d.  %s\n", ii+1, constraint_type);  
            if (!strcmp(constraint_type, "ANGL_EDGE_PARMS"))  
            {  
                UF_CALL(UF_MODL_ask_angle_edge(feat, constraints[ii], 1, fixed1,  
                    fixed2, feature1, feature2, &how_far, &tool, &part));  
                show_constraint_data(tool, part, how_far, fixed1, fixed2, feature1,  
                    feature2, &cplane_csys[9], cplane_csys);  
            }  
            if (!strcmp(constraint_type, "HORZ_DIST_PARMS"))  
            {  
                UF_CALL(UF_MODL_ask_horz_dime(feat, constraints[ii], 1, fixed1,  
                    fixed2, feature1, feature2, &how_far, &tool, &part));  
                show_constraint_data(tool, part, how_far, fixed1, fixed2, feature1,  
                    feature2, &cplane_csys[9], cplane_csys);  
            }  
            if (!strcmp(constraint_type, "PARA_DIST_PARMS"))  
            {  
                UF_CALL(UF_MODL_ask_para_dist(feat, constraints[ii], 1, fixed1,  
                    fixed2, feature1, feature2, &how_far, &tool, &part));  
                show_constraint_data(tool, part, how_far, fixed1, fixed2, feature1,  
                    feature2, &cplane_csys[9], cplane_csys);  
            }  
            if (!strcmp(constraint_type, "PARA_EDGE_PARMS"))  
            {  
                UF_CALL(UF_MODL_ask_para_edge(feat, constraints[ii], 1, fixed1,  
                    fixed2, feature1, feature2, &prllel, &how_far, &tool, &part));  
                ECHO(prllel);  
                show_constraint_data(tool, part, how_far, fixed1, fixed2, feature1,  
                    feature2, &cplane_csys[9], cplane_csys);  
            }  
            if (!strcmp(constraint_type, "PERP_DIST_PARMS"))  
            {  
                UF_CALL(UF_MODL_ask_perp_dist(feat, constraints[ii], 1, fixed1,  
                    fixed2, feature1, feature2, &how_far, &tool, &part));  
                show_constraint_data(tool, part, how_far, fixed1, fixed2, feature1,  
                    feature2, &cplane_csys[9], cplane_csys);  
            }  
            if (!strcmp(constraint_type, "VERT_DIST_PARMS"))  
            {  
                UF_CALL(UF_MODL_ask_vert_dime(feat, constraints[ii], 1, fixed1,  
                    fixed2, feature1, feature2, &how_far, &tool, &part));  
                show_constraint_data(tool, part, how_far, fixed1, fixed2, feature1,  
                    feature2, &cplane_csys[9], cplane_csys);  
            }  
            UF_free(constraint_type);  
        }  
        uc1601(feat_name, TRUE);  
        UF_DISP_refresh();  
        UF_free(feat_name);  
        UF_free(constraints);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            *feats;  
        while ((n_feats = select_features("Select features", &feats)) != 0)  
        {  
            for (ii = 0; ii < n_feats; ii++)  
                report_feature_constraints(feats[ii]);  
            UF_free(feats);  
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

> 这段代码是用于NX的二次开发，其主要功能是选择特征并报告特征约束的相关信息。以下是代码的主要功能介绍：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 选择特征：定义了一个选择特征的函数select_features，用于让用户选择特征，并获取特征列表。
> 3. 分配内存：定义了一个内存分配函数allocate_memory，用于动态分配内存。
> 4. 创建数组：定义了一个创建数组的函数make_an_array，用于将列表转换为数组。
> 5. 显示临时点：定义了一个显示临时点的函数display_temporary_point，用于在模型上显示一个临时点。
> 6. 显示圆锥头：定义了一个显示圆锥头的函数display_conehead，用于在模型上显示一个圆锥头。
> 7. 报告标签类型和子类型：定义了一个报告标签类型和子类型的函数report_tag_type_and_subtype。
> 8. 显示约束数据：定义了一个显示约束数据的函数show_constraint_data，用于在模型上显示约束相关的点。
> 9. 报告特征约束：定义了一个报告特征约束的函数report_feature_constraints，用于查询特征的所有约束并显示。
> 10. 主函数：定义了一个主函数do_it，用于循环选择特征并报告约束。
> 11. UF初始化和终止：在ufusr函数中，先初始化UF，然后调用主函数，最后终止UF。
> 12. 卸载请求：定义了一个卸载请求函数ufusr_ask_unload，用于请求立即卸载。
>
> 通过这些函数，该代码实现了选择特征并查询约束信息的功能，为用户提供了查看和检查约束的便利。
>
