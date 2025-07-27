### 【0927】create straight pipe routing with elbows 创建带有弯头的直管路

#### 代码

```cpp
    /*HEAD CREATE_STRAIGHT_PIPE_ROUTING_WITH_ELBOWS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_route.h>  
    #include <uf_dirpath.h>  
    #include <uf_assem.h>  
    #include <uf_attr.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。 */  
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
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
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
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            n_std_part_charx,  
            n_std_part_matches,  
            n_stock_charx,  
            n_stock_matches,  
            num_places,  
            num_rcps,  
            num_segs,  
            style = UF_ROUTE_STYLE_SIMPLE;  
        tag_t  
            anchor,  
            cross,  
            dir_path,  
            fit_part,  
            inst_id,  
            occ,  
            rcp,  
            *rcps,  
            seg,  
            *segs,  
            stock_data,  
            work_part,  
            work_part_comp,  
            work_part_occ;  
        double  
            angle,  
            ccw[3],  
            csys[6] = { 1.0, 0.0,  0.0, 0.0, 1.0, 0.0 },  
            in_vec[3],  
            out_vec[3],  
            origin[3] = { 0.0, 0.0, 0.0 },  
            new_origin[3],  
            new_csys[6],  
            pos1[3],  
            pos2[3],  
            pos3[3];  
        char  
            *app_view_name = NULL,  
            *app_view_symb = "UGII_BASE_APP_VIEW",  
            full_name[MAX_FSPEC_SIZE],  
            msg[133],  
            *part_search_symb = "UGII_ROUTING_PART_PATH",  
            *std_part_material = "STEEL",  
            *std_part_rating   = "40",  
            *stock_material = "STEEL",  
            *stock_schedule = "40";  
        uf_list_p_t  
                rcp_list,  
                seg_list,  
                tmp;  
        UF_PART_load_status_t  
            error_code ;  
        UF_ROUTE_charx_t  
            std_part_charx[4],  
            stock_charx[3];  
        UF_ROUTE_part_lib_part_p_t  
            std_part_matches,  
            stock_matches;  
        UF_ROUTE_application_view_p_t  
            app_view,  
            old_app_view;  
        UF_ROUTE_place_solution_p_t  
            *places;  
     /* Load and set the application view *** Routing -> Base 里海译:加载并设置应用程序视图 Routing -> Base */  
        old_app_view = UF_ROUTE_ask_current_app_view( );  
        UF_CALL(UF_translate_variable( app_view_symb, &app_view_name ));  
        UF_CALL(UF_ROUTE_load_app_view( app_view_name, &app_view ));  
        UF_CALL(UF_ROUTE_set_current_app_view( app_view ));  
     /* Set the search path for locating Routing (Base) parts / stock 里海译:设置搜索路径以定位路由(基)部件/库存 */  
        UF_CALL(UF_DIRPATH_create_from_env ( part_search_symb, &dir_path ));  
        UF_CALL(UF_ROUTE_set_part_search_path ( dir_path ));  
        UF_CALL(UF_MODL_create_list(&rcp_list));  
        while (sprintf(msg, "Indicate Routing Point #%d - Cancel when done", ++inx)  
            && specify_point(msg, pos1))  
        {  
            UF_CALL(UF_ROUTE_create_rcp_position(pos1, &rcp));  
            UF_CALL(UF_MODL_put_list_item(rcp_list, rcp));  
        }  
     /* Create the Segments through these RCPs 里海译:通过这些RCP创建段 */  
        UF_CALL(UF_MODL_create_list(&seg_list));  
        for (tmp = rcp_list; tmp->next != NULL; tmp = tmp->next)  
        {  
            UF_CALL(UF_ROUTE_create_seg_thru_rcps(tmp->eid, tmp->next->eid, &seg));  
            UF_CALL(UF_MODL_put_list_item(seg_list, seg));  
        }  
        num_rcps = make_an_array(&rcp_list, &rcps);  
        num_segs = make_an_array(&seg_list, &segs);  
     /* Set up the characteristics for the stock we wish to assign 里海译:设置我们希望分配的股票的特征 */  
        stock_charx[0].type = UF_EPLIB_CHARX_TYPE_REAL;  
        strcpy (stock_charx[0].title, "DIAMETER");  
        stock_charx[0].value.r_value = 0.75;  
        stock_charx[1].type = UF_EPLIB_CHARX_TYPE_STR;  
        strcpy (stock_charx[1].title, "MATERIAL");  
        stock_charx[1].value.s_value = stock_material;  
        stock_charx[2].type = UF_EPLIB_CHARX_TYPE_STR;  
        strcpy (stock_charx[2].title, "SCHEDULE");  
        stock_charx[2].value.s_value = stock_schedule;  
        n_stock_charx = sizeof( stock_charx ) / sizeof( stock_charx[0] );  
     /* Find the stock we want in the Routing Part Library 里海译:在路由部件库中找到我们需要的库存。 */  
        UF_CALL(UF_ROUTE_match_charx_in_plib( "STOCK", n_stock_charx, stock_charx,  
                                      &n_stock_matches, &stock_matches ));  
     /* Locate (or load) the stock data which matches our criteria 里海译:根据我们的标准定位（或加载）股票数据。 */  
        UF_CALL(UF_ROUTE_load_stock_by_charx( &stock_matches[0], "", style,  
                                      &stock_data, &anchor, &cross ));  
        UF_CALL(UF_ROUTE_free_match_results( n_stock_matches, stock_matches ));  
        UF_CALL(UF_ROUTE_assign_stock( stock_data, anchor, cross, num_segs, segs ));  
     /*  Place standard part elbows at the RCPs 里海译:在RCPs位置安装标准弯头部件。 */  
        for (inx = 1; inx < num_rcps - 1; inx++)  
        {  
        /*  Set up the characteristics for the Standard part we wish to place 里海译:设置我们希望放置的标准件的特性 */  
            std_part_charx[0].type = UF_EPLIB_CHARX_TYPE_REAL;  
            strcpy (std_part_charx[0].title, "DIAMETER");  
            std_part_charx[0].value.r_value = 0.75;  
            std_part_charx[1].type = UF_EPLIB_CHARX_TYPE_REAL;  
            strcpy (std_part_charx[1].title, "ELBOW_ANG");  
        /*  Calculate the angle at the joint 里海译:计算关节处的角度 */  
            UF_CALL(UF_ROUTE_ask_rcp_position(rcps[inx-1], pos1));  
            UF_CALL(UF_ROUTE_ask_rcp_position(rcps[inx], pos2));  
            UF_CALL(UF_ROUTE_ask_rcp_position(rcps[inx+1], pos3));  
            UF_VEC3_sub(pos2, pos1, in_vec);  
            UF_VEC3_sub(pos3, pos2, out_vec);  
            UF_VEC3_cross(out_vec, in_vec, ccw);  
            UF_VEC3_angle_between(out_vec, in_vec, ccw, &angle);  
            std_part_charx[1].value.r_value = angle * RADEG;  
            std_part_charx[2].type = UF_EPLIB_CHARX_TYPE_STR;  
            strcpy (std_part_charx[2].title, "FITTING_MATERIAL");  
            std_part_charx[2].value.s_value = std_part_material;  
            std_part_charx[3].type = UF_EPLIB_CHARX_TYPE_STR;  
            strcpy (std_part_charx[3].title, "RATING");  
            std_part_charx[3].value.s_value = std_part_rating;  
            n_std_part_charx = sizeof(std_part_charx) / sizeof(std_part_charx[0]);  
            UF_CALL(UF_ROUTE_match_charx_in_plib( "ROUND_ELBOW", n_std_part_charx,  
                                          std_part_charx, &n_std_part_matches,  
                                          &std_part_matches ));  
            if (n_std_part_matches == 0) continue;  
            UF_CALL(UF_ROUTE_load_part_by_charx( std_part_matches[0].num_charx,  
                std_part_matches[0].charx, &fit_part ));  
         /* Add the elbow as a component of the work part 里海译:添加肘部作为工件部件。 */  
            UF_PART_ask_part_name( fit_part, full_name );  
            work_part = UF_ASSEM_ask_work_part( );  
            UF_CALL(UF_ASSEM_add_part_to_assembly( work_part, full_name, NULL,  
                                           NULL, origin, csys, -1,  
                                           &inst_id, &error_code ));  
            /*  
                For the following Routing operations of setting the characteristics  
                of the fitting part and for placing it within the Routing, we  
                use the part occurrence of the fitting in the part occurrence tree  
                that is "rooted" at the work part. This is because the Routing  
                characteristics and placement functions expect this particular  
                part occurrence.  
            */  
            work_part_comp = UF_ASSEM_ask_parent_of_instance( inst_id );  
            work_part_occ = UF_ASSEM_ask_root_part_occ( work_part_comp );  
            occ = UF_ASSEM_ask_part_occ_of_inst( work_part_occ, inst_id ) ;  
            /*  
                We now have the part occurrence of the fitting in the work part's  
                part occurrence tree. So we can attach the Routing characteristics  
                to this part occurrence and place this occurrence within the  
                Routing.  
            */  
            UF_CALL(UF_ROUTE_set_characteristics( occ,  
                std_part_matches[0].num_charx, std_part_matches[0].charx ));  
         /* Place the elbow at the RCPs 里海译:将肘部置于RCPs上 */  
            num_places = 0; places = NULL;  
            UF_CALL(UF_ROUTE_solve_places( rcps[inx], occ, &num_places, &places ));  
            UF_ROUTE_ask_places_transform( places[0], new_origin, new_csys );  
            UF_CALL(UF_ASSEM_reposition_instance( inst_id, new_origin, new_csys ));  
            UF_CALL(UF_ROUTE_free_places( num_places, places ));  
            UF_CALL(UF_ROUTE_set_part_in_stock( occ ));  
        }  
        if (num_rcps > 0) UF_free(rcps);  
        if (num_segs > 0) UF_free(segs);  
        UF_CALL(UF_ROUTE_unload_app_view( app_view ));  
        UF_CALL(UF_ROUTE_set_current_app_view( old_app_view ));  
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

> 这段NX二次开发代码的主要功能是创建一个直管路线，并在指定位置添加弯头。代码的主要步骤包括：
>
> 1. 加载并设置应用程序视图：加载并设置Routing -> Base应用程序视图。
> 2. 设置搜索路径：设置路由部件和库存的搜索路径。
> 3. 指定路由点：使用交互方式指定多个路由点，并创建相应的RCP(路由控制点)。
> 4. 创建通过RCP的路线段：通过指定的RCP创建路线段。
> 5. 设置和匹配库存：设置库存特征，并在库存库中匹配，然后加载匹配的库存数据。
> 6. 为路线段分配库存：将匹配的库存分配给所有路线段。
> 7. 设置标准件特征：为每个RCP设置标准弯头的特征，包括直径、角度等。
> 8. 匹配并加载标准件：在标准件库中匹配弯头，并加载匹配的弯头。
> 9. 将标准件添加到装配体：将匹配的弯头作为组件添加到当前装配体。
> 10. 设置标准件特征并放置：为添加的弯头设置特征，并在RCP处放置弯头。
> 11. 释放资源：释放相关资源，恢复原始应用程序视图。
>
> 通过以上步骤，代码实现了创建直管路线并在指定位置添加弯头的目的。
>
