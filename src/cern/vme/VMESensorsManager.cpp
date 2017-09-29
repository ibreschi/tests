#include "VMESensorsManager.h"
#include "Event.h"
#include "Sensor.h"
#include "XmlEncoder.h"

#include <iostream>
#include <algorithm>
#include <stdexcept>

namespace cern
{

VMESensorsManager::VMESensorsManager() :
				stop_(false)
{
}

VMESensorsManager::~VMESensorsManager()
{
}

void VMESensorsManager::addSensor(uint16_t address, bool type)
{
	if (sensors_.size() < 8)
	{
		sensors_.push_back(SensorPtr(new Sensor(address, queue_)));
	} else
		throw std::runtime_error("Max sensor reached !");
}

void VMESensorsManager::removeSensor(uint16_t address)
{
	// how to do this?
	SensorVector::iterator it = std::remove(sensors_.begin(), sensors_.end(), Sensor(address));
	sensors_.erase(it, sensors_.end());
}

void VMESensorsManager::setOutStream(std::ostream& stream)
{
	encoder_ = std::unique_ptr<Encoder>(new XmlEncoder(stream));
}

std::vector<uint16_t> VMESensorsManager::getSensorList() const
{
	std::vector<uint16_t> result;
	for (auto& s : sensors_)
		result.push_back(s->getAddress());
	return result;
}

void VMESensorsManager::stop()
{
	stop_ = true;
	//notify all with a signal
}

void VMESensorsManager::setConversionFactors(uint16_t address, double scalingFactor, double offset)
{
	SensorVector::iterator it = std::find(sensors_.begin(), sensors_.end(), Sensor(address));
	if (it != sensors_.end())
		(*it)->setCalibrator(scalingFactor, offset);
	else
		std::cout << "Element not found in myvector\n";
}

void VMESensorsManager::run()
{
	while (!stop_)
	{
		// Notify all with a signal
//		for (Sensor s : sensors_)
//		{
//			s.read();
//		}

		// where should I put this??
		Event state = queue_.dequeue();
		if (encoder_)
		{
			// how do we pass the state to the encoder ?state
			encoder_->encode();
		}
		//		wait
		//	for 1 second timer arrives
	}
}
} // namespace cern
