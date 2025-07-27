### 【2040】report csys to csys offsets 报告坐标系到坐标系偏移量

#### 代码

```cpp
    static void map_abs2csys(tag_t input_csys, double loc[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[16];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, csys));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(loc, mx);  
        UF_VEC3_sub(loc, csys, loc);  
    }  
    static void map_vec_abs2csys(tag_t input_csys, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 },  
            origin[3],  
            mx[16];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
    }  
    static void transform_abs2csys(double orig[3], double csys[9], double mx[16])  
    {  
        double  
            tx1[12],  
            tx2[12];  
    /*  Invert the rotation from the destination matrix 译:Invert the rotation from the destination matrix的中文翻译是：反转目标矩阵的旋转。 */  
        tx2[0] = csys[0];  
        tx2[1] = csys[3];  
        tx2[2] = csys[6];  
        tx2[3] = 0;  
        tx2[4] = csys[1];  
        tx2[5] = csys[4];  
        tx2[6] = csys[7];  
        tx2[7] = 0;  
        tx2[8] = csys[2];  
        tx2[9] = csys[5];  
        tx2[10] = csys[8];  
        tx2[11] = 0;  
    /*  set up to translate from abs to the destination csys 译:从abs坐标系转换到目标坐标系。 */  
        FTN(uf5943)(orig, tx1);  
    /*  combine this with the inverted rotation csys above 译:结合上述倒转的旋转坐标系统。 */  
        FTN(uf5942)(tx2, tx1, mx);  
    }  
    static void report_csys_to_csys_offsets(tag_t ref_csys, tag_t dest_csys)  
    {  
        double  
            angle_x,  
            angle_y,  
            angle_z,  
            delta[3],  
            from_orig[3],  
            mx[16],  
            to_csys[9],  
            to_orig[3],  
            zero[3] = { 0,0,0 };  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(ref_csys, &csys_mx, from_orig));  
        UF_CALL(UF_CSYS_ask_csys_info(dest_csys, &csys_mx, to_orig));  
        map_abs2csys(ref_csys, from_orig);  
        map_abs2csys(ref_csys, to_orig);  
        UF_VEC3_sub(to_orig, from_orig, delta);  
        ECHO("Translation:\n");  
        ECHO("  X = %f\n", delta[0]);  
        ECHO("  Y = %f\n", delta[1]);  
        ECHO("  Z = %f\n", delta[2]);  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, to_csys));  
        map_vec_abs2csys(ref_csys, to_csys);  
        map_vec_abs2csys(ref_csys, &to_csys[3]);  
        map_vec_abs2csys(ref_csys, &to_csys[6]);  
        transform_abs2csys(zero, to_csys, mx);  
        angle_z = atan2(mx[4], mx[0]);  
        angle_y = -asin(mx[8]);  
        angle_x = atan2(mx[9], mx[10]);  
        ECHO("Rotation:\n");  
        ECHO("  Angle X = %f\n", angle_x * RADEG);  
        ECHO("  Angle Y = %f\n", angle_y * RADEG);  
        ECHO("  Angle Z = %f\n", angle_z * RADEG);  
    }

```

#### 代码解析

> 这段NX二次开发代码主要用于计算两个坐标系之间的平移和旋转差值。
>
> 主要功能包括：
>
> 1. map_abs2csys：将一个点从绝对坐标系转换到指定的坐标系。
> 2. map_vec_abs2csys：将一个向量从绝对坐标系转换到指定的坐标系。
> 3. transform_abs2csys：计算从绝对坐标系到指定坐标系的变换矩阵。
> 4. report_csys_to_csys_offsets：计算两个坐标系之间的平移差值和旋转差值，并打印输出。
>
> 具体步骤如下：
>
> 1. 通过UF_CSYS_ask_csys_info获取两个坐标系的原始信息。
> 2. 使用map_abs2csys将坐标系的原点转换到绝对坐标系。
> 3. 计算两个坐标系原点之间的平移差值。
> 4. 使用map_vec_abs2csys将坐标系的三个轴向量转换到绝对坐标系。
> 5. 使用transform_abs2csys计算从绝对坐标系到目标坐标系的变换矩阵。
> 6. 从变换矩阵中提取旋转角度。
> 7. 打印输出两个坐标系之间的平移和旋转差值。
>
> 通过这段代码，可以方便地计算和比较两个坐标系之间的差异，对于坐标系管理和坐标转换非常有用。
>
