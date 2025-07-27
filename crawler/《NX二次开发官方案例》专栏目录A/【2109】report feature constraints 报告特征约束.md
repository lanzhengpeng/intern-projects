### 【2109】report feature constraints 报告特征约束

#### 代码

```cpp
    static void report_feature_constraints(tag_t feat)  
    {  
        int  
            ii,  
            n_constraints,  
            parallel;  
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
                    fixed2, feature1, feature2, &parallel, &how_far, &tool, &part));  
                ECHO(parallel);  
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

```

#### 代码解析

> 这段代码用于NX的二次开发，其主要功能是查询和显示NX特征中的各种约束。
>
> 代码的主要逻辑包括：
>
> 1. 获取特征名。
> 2. 查询特征的所有约束，并存储在数组中。
> 3. 遍历约束数组，根据不同的约束类型，调用不同的函数查询详细信息，并输出显示。
> 4. 显示完成后，释放内存。
>
> 其中，各种约束类型包括角度约束、水平距离约束、平行距离约束、平行边约束、垂直距离约束和垂直距离约束。
>
> 该代码利用NX提供的UF_MODL_ask_XXX系列函数来查询特征中的各种约束，并使用UF_DISP系列函数来显示结果。它通过二次开发扩展了NX的约束查询功能，方便用户获取特征之间的约束关系。
>
