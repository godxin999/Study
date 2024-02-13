export module screen;

export class Screen {
public:
	static int GetWidth() {
		return m_Width;
	}
	static int GetHeight() {
		return m_Height;
	}
	static float GetAspectRatio() {
		return m_AspectRatio;
	}
	static void SetWidth(int width) {
		m_Width = width;
		CalculateScreenSizeRatio();
	}
	static void SetHeight(int height) {
		m_Height = height;
		CalculateScreenSizeRatio();
	}
	static void SetWidthHeight(int width, int height) {
		m_Width = width;
		m_Height = height;
		CalculateScreenSizeRatio();
	}
private:
	static void CalculateScreenSizeRatio() {
		m_AspectRatio = m_Width * 1.0f / m_Height;
	}
private:
	inline static int m_Width{800};
	inline static int m_Height{600};
	inline static float m_AspectRatio{1.778f};
};