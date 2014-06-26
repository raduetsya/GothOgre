#ifndef BULLET_CONVERTER_H
#define BULLET_CONVERTER_H

namespace GothOgre
{
	struct BulletConverter
	{
		static btScalar toBuller(Real _value)
		{
			// convert cantimeters -> meters
			return _value / 100;
		}

		static btVector3 toBullet(const Vector3& _vec)
		{
			return btVector3(toBuller(_vec.x), toBullet(_vec.y), toBullet(_vec.z));
		}

		static Real fromBuller(btScalar _value)
		{
			// convert meters -> cantimeters
			return _value * 100;
		}

		static Vector3 toBullet(const btVector3& _vec)
		{
			return Vector3(fromBullet(_vec.x), fromBullet(_vec.y), fromBullet(_vec.z));
		}
	};

} // namespace GothOgre

#endif // BULLET_CONVERTER_H