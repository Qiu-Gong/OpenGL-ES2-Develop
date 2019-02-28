package qiugong.com.myapplication.data;

import android.opengl.GLES20;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.ShortBuffer;

import qiugong.com.myapplication.Constants;

/**
 * @author qzx 2019/2/27.
 */
public class IndexBuffer {
    private final int bufferId;

    public IndexBuffer(short[] indexData) {
        final int buffers[] = new int[1];
        GLES20.glGenBuffers(buffers.length, buffers, 0);
        if (buffers[0] == 0) {
            throw new RuntimeException("Could not create a new vertex buffer object.");
        }
        bufferId = buffers[0];

        GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, buffers[0]);

        ShortBuffer indexArray = ByteBuffer
                .allocateDirect(indexData.length * Constants.BYTES_PER_SHORT)
                .order(ByteOrder.nativeOrder())
                .asShortBuffer()
                .put(indexData);
        indexArray.position(0);

        GLES20.glBufferData(GLES20.GL_ELEMENT_ARRAY_BUFFER, indexArray.capacity() * Constants.BYTES_PER_SHORT, indexArray, GLES20.GL_STATIC_DRAW);
        GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    public void setVertexAttribPointer(int dataOffset, int attributeLocation, int componentCount, int stride) {
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, bufferId);
        GLES20.glVertexAttribPointer(attributeLocation, componentCount, GLES20.GL_FLOAT, false, stride, dataOffset);
        GLES20.glEnableVertexAttribArray(attributeLocation);
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
    }

    public int getBufferId() {
        return bufferId;
    }
}
