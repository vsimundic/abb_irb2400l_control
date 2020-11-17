//Astra 
//#include <C:/Users/Cupec/Downloads/AstraSDK-0.5.0-vs2013-win64/AstraSDK-0.5.0-20160426T103143Z-vs2013-win64/samples/vs2013/thirdparty/SFML-min-64/include/SFML/Graphics.hpp>
#include <astra_core/astra_core.hpp>
#include <astra/astra.hpp>
#include <string.h>

extern unsigned char *img;
extern int16_t *imgD;

class ColorFrameListener : public astra::FrameListener
{
public:
	
	ColorFrameListener()
	{
		prev_ = ClockType::now();
	}

	//void init_texture(int width, int height)
	//{
	//	if (displayBuffer_ == nullptr || width != displayWidth_ || height != displayHeight_)
	//	{
	//		displayWidth_ = width;
	//		displayHeight_ = height;

	//		// texture is RGBA
	//		int byteLength = displayWidth_ * displayHeight_ * 4;

	//		displayBuffer_ = BufferPtr(new uint8_t[byteLength]);
	//		std::memset(displayBuffer_.get(), 0, byteLength);

	//		texture_.create(displayWidth_, displayHeight_);
	//		sprite_.setTexture(texture_);
	//		sprite_.setPosition(0, 0);
	//	}
	//}

	void check_fps()
	{
		const float frameWeight = .2f;

		const ClockType::time_point now = ClockType::now();
		const float elapsedMillis = std::chrono::duration_cast<DurationType>(now - prev_).count();

		elapsedMillis_ = elapsedMillis * frameWeight + elapsedMillis_ * (1.f - frameWeight);
		prev_ = now;

		const float fps = 1000.f / elapsedMillis;

		const auto precision = std::cout.precision();

		std::cout << std::fixed
			<< std::setprecision(1)
			<< fps << " fps ("
			<< std::setprecision(1)
			<< elapsedMillis_ << " ms)"
			<< std::setprecision(precision)
			<< std::endl;
	}

	virtual void on_frame_ready(astra::StreamReader& reader, astra::Frame& frame) override
	{
		const astra::ColorFrame colorFrame = frame.get<astra::ColorFrame>();

		int width = colorFrame.width();
		int height = colorFrame.height();

		//init_texture(width, height);

		const astra::RgbPixel* colorData = colorFrame.data();
		
		for (int i = 0; i < width * height; i++)
		{
			int rgbaOffset = i * 3;
			img[rgbaOffset] = colorData[i].b;
			img[rgbaOffset + 1] = colorData[i].g;
			img[rgbaOffset + 2] = colorData[i].r;
		}

		const astra::DepthFrame depthFrame = frame.get<astra::DepthFrame>();

		memcpy(imgD, depthFrame.data(), width*height*sizeof(int16_t));

		//for (int i = 0; i < width * height; i++)
		//{
		//	int rgbaOffset = i * 4;
		//	displayBuffer_[rgbaOffset] = colorData[i].r;
		//	displayBuffer_[rgbaOffset + 1] = colorData[i].g;
		//	displayBuffer_[rgbaOffset + 2] = colorData[i].b;
		//	displayBuffer_[rgbaOffset + 3] = 255;
		//}

		//texture_.update(displayBuffer_.get());
		check_fps();
	}

	/*void drawTo(sf::RenderWindow& window)
	{
	if (displayBuffer_ != nullptr)
	{
	float imageScale = window.getView().getSize().x / displayWidth_;
	sprite_.setScale(imageScale, imageScale);
	window.draw(sprite_);
	}
	}*/

private:
	using DurationType = std::chrono::milliseconds;
	using ClockType = std::chrono::high_resolution_clock;

	ClockType::time_point prev_;
	float elapsedMillis_{ .0f };

	//sf::Texture texture_;
	//sf::Sprite sprite_;

	using BufferPtr = std::unique_ptr<uint8_t[]>;
	BufferPtr displayBuffer_{ nullptr };

	int displayWidth_{ 0 };
	int displayHeight_{ 0 };
};