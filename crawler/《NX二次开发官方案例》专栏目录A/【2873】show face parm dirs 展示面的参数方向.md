### 【2873】show face parm dirs 展示面的参数方向

#### 代码

```cpp
    static void show_face_parm_dirs(tag_t face)  
    {  
        double  
            junk[3],  
            origin[3],  
            parm[2],  
            uvs[4],  
            u_dir[3],  
            v_dir[3];  
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
        parm[0] = uvs[0];  
        parm[1] = uvs[2];  
        UF_CALL(UF_MODL_ask_face_props(face, parm, origin, u_dir, v_dir,  
            junk, junk, junk, junk));  
        UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, u_dir, 0, "U");  
        UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, v_dir, 0, "V");  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要用于显示指定面片的UV参数方向。具体介绍如下：
>
> 1. 定义了一些double类型的数组，用于存储中间结果和最终结果。
> 2. 调用UF_MODL_ask_face_uv_minmax函数，获取指定面片的UV参数的最小值和最大值，并存储在uvs数组中。
> 3. 从uvs数组中提取最小U和最大V，并存储在parm数组中。
> 4. 调用UF_MODL_ask_face_props函数，获取面片的属性，包括参数原点、U方向和V方向等，并存储在对应的数组中。
> 5. 使用UF_DISP_labeled_conehead函数，在所有激活的视图上显示参数原点、U方向和V方向，其中U方向和V方向分别用标签“U”和“V”标注。
> 6. 整个函数通过UF_CALL宏调用NX Open C++ API函数，并通过UF_DISP宏显示结果。
> 7. 函数的目的是为了显示面片的UV参数方向，便于理解面片的参数化情况。
>
> 这段代码实现了在NX中显示面片的UV参数方向的功能，对理解面片的参数化情况很有帮助。
>
