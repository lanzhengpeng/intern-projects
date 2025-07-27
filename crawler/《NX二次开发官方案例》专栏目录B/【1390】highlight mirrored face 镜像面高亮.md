### 【1390】highlight mirrored face 镜像面高亮

#### 代码

```cpp
    static void highlight_mirrored_face(tag_t face)  
    {  
        int  
            ii,  
            jj,  
            n_feats,  
            n_items;  
        char  
            *feat_type;  
        tag_t  
            *feats,  
            mirror,  
            *results,  
            *sources;  
        n_feats = ask_face_feats(face, &feats);  
        for (ii = 0; ii < n_feats; ii++)  
        {  
            UF_CALL(UF_MODL_ask_feat_type(feats[ii], &feat_type));  
            if ((!strcmp(feat_type, "MIRROR")) &&  
                ! UF_CALL(UF_WAVE_map_link_geom_to_source(feats[ii], face, TRUE,  
                    &mirror)))  
            {  
            /*  Begin work around to PR 4244233 译:开始围绕PR 4244233展开工作。 */  
                if (mirror == NULL_TAG)  
                {  
                    UF_CALL(UF_WAVE_ask_linked_feature_map(feats[ii], TRUE,  
                        &n_items, &sources, &results));  
                    for (jj = 0; jj < n_items; jj++)  
                    {  
                        if (results[jj] == face)  
                        {  
                            mirror = sources[jj];  
                            break;  
                        }  
                    }  
                    if (n_items > 0)  
                    {  
                        UF_free(sources);  
                        UF_free(results);  
                    }  
                }  
            /*  End work around to PR 4244233 译:结束对PR 4244233的绕行工作。 */  
                UF_DISP_set_highlight(face, TRUE);  
                UF_DISP_set_highlight(mirror, TRUE);  
                uc1601("Mirrored face highlighted", TRUE);  
                UF_DISP_set_highlight(face, FALSE);  
                UF_DISP_set_highlight(mirror, FALSE);  
            }  
            UF_free(feat_type);  
        }  
        if (n_feats > 0) UF_free(feats);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，用于高亮显示NX模型中的一个镜像特征及其对应的镜像面。其主要功能如下：
>
> 1. 获取指定面的所有特征，并遍历这些特征。
> 2. 检查每个特征是否为镜像特征（类型为"MIRROR"）。
> 3. 如果是镜像特征，获取其对应的镜像面。
> 4. 如果获取不到镜像面，通过绕过PR 4244233的缺陷，从特征映射关系中手动找到镜像面。
> 5. 高亮显示指定面和镜像面。
> 6. 输出提示信息。
> 7. 清除高亮显示。
> 8. 释放内存。
>
> 该代码主要用于NX模型处理，可以用于检查和验证模型的镜像特征。通过绕过PR 4244233的缺陷，确保在NX旧版本中也能正常工作。
>
