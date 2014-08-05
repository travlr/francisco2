#ifndef RAV_OBSTACLE_H
#define RAV_OBSTACLE_H

#include <vector>
#include <Coord.h>
#include <AnnotationManager.h>

using Veins::AnnotationManager;
using Veins::AnnotationManagerAccess;

/**
 * stores information about an RavObstacle for RavObstacleControl
 */

class RavObstacle {
	public:
		typedef std::vector<Coord> Coords;

        RavObstacle(std::string id, double attenuationPerWall, double attenuationPerMeter);

		void setShape(Coords shape);
		const Coords& getShape() const;
		const Coord getBboxP1() const;
		const Coord getBboxP2() const;

		double calculateAttenuation(const Coord& senderPos, const Coord& receiverPos) const;

		AnnotationManager::Annotation* visualRepresentation;

	protected:
		std::string id;
		double attenuationPerWall; /**< in dB. Consumer Wi-Fi vs. an exterior wall will give approx. 50 dB */
		double attenuationPerMeter; /**< in dB / m. Consumer Wi-Fi vs. an interior hollow wall will give approx. 5 dB */
		Coords coords;
		Coord bboxP1;
		Coord bboxP2;
};


#endif
