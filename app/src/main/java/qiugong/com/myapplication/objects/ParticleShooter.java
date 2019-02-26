package qiugong.com.myapplication.objects;

import android.opengl.Matrix;

import java.util.Random;

import qiugong.com.myapplication.util.Geometry;

/**
 * @author qzx 2019/2/26.
 */
public class ParticleShooter {

    private final Geometry.Point position;
    private final Geometry.Vector direction;
    private final int color;

    private final float angleVariance;
    private final float speedVariance;

    private final Random random = new Random();

    private float[] rotationMatrix = new float[16];
    private float[] directionVector = new float[4];
    private float[] resultVector = new float[4];

    public ParticleShooter(Geometry.Point position, Geometry.Vector direction, int color,
                           float angleVarianceInDegrees, float speedVariance) {
        this.position = position;
        this.direction = direction;
        this.color = color;

        this.angleVariance = angleVarianceInDegrees;
        this.speedVariance = speedVariance;

        directionVector[0] = direction.x;
        directionVector[1] = direction.y;
        directionVector[2] = direction.z;
    }

    public void addParticles(ParticleSystem particleSystem, float currentTime, int count) {
        for (int i = 0; i < count; i++) {
            Matrix.setRotateEulerM(rotationMatrix, 0,
                    (random.nextFloat() - 0.5f) * angleVariance,
                    (random.nextFloat() - 0.5f) * angleVariance,
                    (random.nextFloat() - 0.5f) * angleVariance);

            Matrix.multiplyMV(resultVector, 0,
                    rotationMatrix, 0,
                    directionVector, 0);

            float speedAdjustment = 1f + random.nextFloat() * speedVariance;

            Geometry.Vector thisDirection = new Geometry.Vector(
                    resultVector[0] * speedAdjustment,
                    resultVector[1] * speedAdjustment,
                    resultVector[2] * speedAdjustment);

            particleSystem.addParticle(position, color, thisDirection, currentTime);
        }
    }
}
