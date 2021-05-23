/*
 * easy_image.h
 * Copyright (C) 2011  Daniel van den Akker
 */
#ifndef EASY_IMAGE_INCLUDED
#define EASY_IMAGE_INCLUDED
#include <cstdint>
#include <vector>
#include <iostream>
#include <list>
#include "z_buffer.h"
#include "vector3d.h"

class ZBuffer;

namespace img
{
	class Color
	{
		//a safety warning to all of you: Do *NOT* rearrange the 'color components' in this class
		//easyimage expects these three fields to be the *first* fields in the class AND expects
		//them to be in *this* order
		//if you alter the arrangement, the generated BMP files will contain garbage
		public:
			uint8_t blue;
			uint8_t green;
			uint8_t red;

			Color();

			Color(uint8_t r, uint8_t g, uint8_t b);

			~Color();
	};

	class UnsupportedFileTypeException: public std::exception
	{
		private:
			std::string message;

		public:
			explicit UnsupportedFileTypeException(std::string const& msg);
			UnsupportedFileTypeException(const UnsupportedFileTypeException &original);
			virtual ~UnsupportedFileTypeException() noexcept;
			UnsupportedFileTypeException& operator=(const UnsupportedFileTypeException &original);
			virtual const char *what() const noexcept;
	};

	class EasyImage
	{
		public:
			EasyImage();
			EasyImage(unsigned int width, unsigned int height, Color const &color = Color());
			EasyImage(EasyImage const& img);
			virtual ~EasyImage();
			EasyImage& operator=(EasyImage const& img);
			unsigned int get_width() const;
			unsigned int get_height() const;
			Color& operator()(unsigned int x, unsigned int y);
			Color const& operator()(unsigned int x, unsigned int y) const;
			void clear(Color const &color = Color());
			void draw_line(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, Color const &color);

            void draw_zbuf_line(ZBuffer &zbuffer,
                                unsigned int &x0, unsigned int &y0, double z0,
                                unsigned int &x1, unsigned int &y1, double z1, const Color &color);

            void draw_zbuf_triag(ZBuffer&, Vector3D const& A, Vector3D const& B,
                                Vector3D const& C, double d, double dx, double dy, img::Color &color);

		private:
			friend std::istream& operator>>(std::istream& in, EasyImage & image);
			/**
			 * \brief the width of the image
			 */
			unsigned int width;
			/**
			 * \brief the height of the image
			 */
			unsigned int height;
			/**
			 * \brief the vector containing all pixels
			 */
			std::vector<Color> bitmap;
	};

	/**
	 * \brief Writes an img::EasyImage to an output stream in the BMP file format
	 *
	 * \param out		the std::ostream to write the BMP file to.
	 * \param image		the img::EasyImage to be written to the output stream
	 *
	 * \return		a reference to the output stream the image was written to
	 */
	std::ostream& operator<<(std::ostream& out, EasyImage const& image);
	/**
	 * \brief Reads an img::EasyImage from an input stream.
	 *
	 * Please note: at this point only a limited subset of BMP-file format is supported.
	 * In order to correctly read a BMP file it must:
	 * 	- Be an uncompressed bitmap
	 *	- Only contain one plane
	 * 	- Use 24bits/pixel
	 * If the BMP file-format is not supported an UnsupportedFileTypeException is thrown
	 *
	 * \param in		the input stream to read the bitmap from
	 * \param image		the EasyImage object in which the bitmap must be stored
	 *
	 * \return		a reference to the input stream from which the bitmap was read
	 */
	std::istream& operator>>(std::istream& in, EasyImage& image);
}

#endif /* EASY_IMAGE_INCLUDED */
