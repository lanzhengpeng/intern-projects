### 【1636】matrix csys2csys matrix_csys2csys 将参考坐标系转换为目标坐标系的函数

#### 代码

```cpp
    /* This is convoluted method is obsolete.  See UF_MTX4_csys_to_csys. 译:这个复杂的方法已经过时。请参阅 UF_MTX4_csys_to_csys。 */  
    static void matrix_csys2csys(tag_t ref_csys, tag_t dest_csys, double mx[12])  
    {  
        double  
            csys[9],  
            orig[3],  
            tx1[12],  
            tx2[12],  
            tx3[12],  
            tx4[12],  
            v[3];  
        int  
            ii;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(ref_csys, &csys_mx, orig));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, csys));  
        tx1[0] = csys[0];  
        tx1[1] = csys[1];  
        tx1[2] = csys[2];  
        tx1[3] = 0;  
        tx1[4] = csys[3];  
        tx1[5] = csys[4];  
        tx1[6] = csys[5];  
        tx1[7] = 0;  
        tx1[8] = csys[6];  
        tx1[9] = csys[7];  
        tx1[10] = csys[8];  
        tx1[11] = 0;  
    /*  set up to translate from reference csys back to absolute 译:设置以将参考坐标系转换回绝对坐标系。 */  
        for (ii=0; ii<3; ii++) v[ii] = -orig[ii];  
        FTN(uf5943)(v, tx2);  
    /*  combine this with the rotation matrix from the reference csys 译:将这个与参考坐标系的旋转矩阵结合起来。 */  
        FTN(uf5942)(tx2, tx1, tx3);  
        UF_CALL(UF_CSYS_ask_csys_info(dest_csys, &csys_mx, orig));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, csys));  
    /*  Invert the rotation from the destination matrix 译:将目的地矩阵的旋转进行反转 */  
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
    /*  set up to translate from abs to the destination csys 译:根据文档内容，这句话的翻译是：

设置从ABS坐标系到目标坐标系的转换。 */  
        FTN(uf5943)(orig, tx1);  
    /*  combine this with the inverted rotation csys above 译:结合上述倒转旋转坐标系。 */  
        FTN(uf5942)(tx2, tx1, tx4);  
    /*  and the one from the reference csys 译:根据上下文，"the one from the reference csys"指的是来自参考坐标系统的那一个。因此，"and the one from the reference csys"的意思是“以及来自参考坐标系统的那个”。 */  
        FTN(uf5942)(tx3, tx4, mx);  
    }

```

#### 代码解析

> 这段代码是用于在NX CAD系统中进行坐标系转换的。其功能是从参考坐标系转换到目标坐标系。具体步骤如下：
>
> 1. 获取参考坐标系的旋转矩阵和原点，并构建一个4x3的旋转矩阵。
> 2. 将参考坐标系的原点平移到世界坐标系的原点，得到一个新的4x3矩阵。
> 3. 获取目标坐标系的旋转矩阵和原点，并将旋转矩阵转置得到一个新的4x3矩阵。
> 4. 将目标坐标系的原点平移到世界坐标系的原点，得到一个新的4x3矩阵。
> 5. 将步骤2和步骤4得到的矩阵相乘，得到从参考坐标系到目标坐标系的变换矩阵。
> 6. 将步骤5得到的矩阵与步骤1得到的矩阵相乘，得到最终的变换矩阵。
> 7. 最后，将最终的变换矩阵赋值给mx数组，完成坐标系转换。
>
> 需要注意的是，这段代码使用了NX提供的坐标系管理API，如UF_CSYS_ask_csys_info、UF_CSYS_ask_matrix_values等。另外，代码中注释提到的UF_MTX4_csys_to_csys函数应该是NX后续版本提供的更简单的坐标系转换接口。
>
