#include "..\pch.h"
#include "IppDib.h"

#define DIB_HEADER_MARKER ((WORD) ('M' << 8) | 'B')

IppDib::IppDib()
	: m_nWidth(0), m_nHeight(0), m_nBitCount(0), m_nDibSize(0), m_pDib(NULL)
{
}
IppDib::IppDib(const IppDib& dib)
	: m_nWidth(dib.m_nWidth), m_nHeight(dib.m_nHeight), m_nBitCount(dib.m_nBitCount),
	m_nDibSize(dib.m_nDibSize), m_pDib(NULL)
{
	if (dib.m_pDib != NULL)
	{
		m_pDib = new BYTE[m_nDibSize];
		memcpy(m_pDib, dib.m_pDib, m_nDibSize);
	}
}

IppDib::~IppDib()
{
	if (m_pDib)
		delete[] m_pDib;
}

BOOL IppDib::CreateGrayBitmap(LONG nWidth, LONG nHeight)
{
	if (m_pDib)
		DestroyBitmap();

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nBitCount = 8;

	DWORD dwWidthStep = (m_nWidth * m_nBitCount / 8 + 3) & ~3;
	DWORD dwSizeImage = (m_nHeight * dwWidthStep);
	m_nDibSize = sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * GetPaletteNums()) + dwSizeImage;

	m_pDib = new BYTE[m_nDibSize]; // 이미지 동적할당
	if (m_pDib == NULL)
		return FALSE;

	// BITMAPINFOHEADER 구조체 설정
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	lpbmi->biSize = sizeof(BITMAPINFOHEADER);
	lpbmi->biWidth = m_nWidth;
	lpbmi->biHeight = m_nHeight;
	lpbmi->biPlanes = 1;
	lpbmi->biBitCount = m_nBitCount;
	lpbmi->biCompression = BI_RGB;
	lpbmi->biSizeImage = dwSizeImage;
	lpbmi->biXPelsPerMeter = 0;
	lpbmi->biYPelsPerMeter = 0;
	lpbmi->biClrUsed = 0;
	lpbmi->biClrImportant = 0;

	// 그레이스케일 팔레트 설정
	RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
	for (int i = 0; i < 256; i++) {
		pPal->rgbBlue = (BYTE)i;
		pPal->rgbGreen = (BYTE)i;
		pPal->rgbRed = (BYTE)i;
		pPal->rgbReserved = 0;
		pPal++;
	}

	// 픽셀 데이터 초기화
	BYTE* pData = GetDIBitsAddr();
	memset(pData, 255, dwSizeImage); // 픽셀 값을 0으로 초기화(0으로 초기화 할때만 사용한다.)

	return TRUE;
}

BOOL IppDib::CreateRgbBitmap(LONG nWidth, LONG nHeight)
{
	if (m_pDib)
		DestroyBitmap();

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nBitCount = 24;

	DWORD dwWidthStep = (m_nWidth* m_nBitCount / 8 + 3) & ~3; // 영상의 가로 한 줄을 저장하는 데 필요한 바이트 수
	DWORD dwSizeImage = (m_nHeight * dwWidthStep);
	m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
		return FALSE;

	// BITMAPINFOHEADER 구조체 설정
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	lpbmi->biSize = sizeof(BITMAPINFOHEADER);
	lpbmi->biWidth = m_nWidth;
	lpbmi->biHeight = m_nHeight;
	lpbmi->biPlanes = 1;
	lpbmi->biBitCount = m_nBitCount;
	lpbmi->biCompression = BI_RGB;
	lpbmi->biSizeImage = dwSizeImage;
	lpbmi->biXPelsPerMeter = 0;
	lpbmi->biYPelsPerMeter = 0;
	lpbmi->biClrUsed = 0;
	lpbmi->biClrImportant = 0;

	// 픽셀 데이터 초기화
	BYTE* pData = GetDIBitsAddr();
	memset(pData, 255, dwSizeImage);

	return TRUE;
}

void IppDib::DestroyBitmap()
{
	if (m_pDib)
	{
		delete[] m_pDib;
		m_pDib = NULL;
	}

	m_nWidth = 0;
	m_nHeight = 0;
	m_nBitCount = 0;
	m_nDibSize = 0;

}

BOOL IppDib::Load(const char* filename) {
	const char* ext = strrchr(filename, '.');
	if (!_strcmpi(ext, ".bmp"))
		return LoadBMP(filename);
	else
		return FALSE;
}

BOOL IppDib::Save(const char* filename) {
	const char* ext = strrchr(filename, '.');
	if (!_strcmpi(ext, ".bmp"))
		return SaveBMP(filename);
	else
		return FALSE;
}

// 파일 열기
BOOL IppDib::LoadBMP(const char* filename) {
	FILE *fp = NULL;
	fopen_s(&fp, filename, "rb"); // 2진 모드로 파일을 연다.
	if (!fp)
		return false;

	BITMAPFILEHEADER bmfh; // 비트맵 파일 헤더
	BITMAPINFOHEADER bmih; // 비트맵 정보 헤더

	//파일 입출력 안전하게 작성하기 위해 fread함수가 정상적으로 작성되었는지 검사(예외가 있는 경우 파일을 닫는다)
	// BITMAPFILEHEADER 읽기
	if (fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp) != 1) {
		fclose(fp);
		return false;
	}

	//  BMP 파일임을 나타내는 "BM" 마커가 있는지 확인
	if (bmfh.bfType != DIB_HEADER_MARKER) {
		fclose(fp);
		return false;
	}

	// BITMAPINFOHEADER 읽기
	if (fread(&bmih, sizeof(BITMAPINFOHEADER), 1, fp) != 1) {
		fclose(fp);
		return false;
	}

	m_nWidth = bmih.biWidth;
	m_nHeight = bmih.biHeight;
	m_nBitCount = bmih.biBitCount;

	// 픽셀 데이터 공간 계산
	DWORD dwWidthStep = (DWORD)((m_nWidth * m_nBitCount / 8 + 3) & ~3); // 가로 한 줄 픽셀 정보를 저장하는데 필요한 메모리 바이트 수 저장
	DWORD dwSizeImage = m_nHeight * dwWidthStep; // 픽셀 데이터 값을 저장하는 데 필요한 메모리 공간의 크기

	if (m_nBitCount == 24) // 픽셀 당 비트수 정보를 이용하여 색상 테이블 유무를 파악
		m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;
	else
		m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << m_nBitCount) + dwSizeImage;

	// Packed-DIB 저장할 메모리 공간 동적 할당
	if (m_pDib)
		DestroyBitmap();

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL) {
		fclose(fp);
		return FALSE;
	}

	// 파일로부터 Packed-DIB 크기만큼을 읽기
	fseek(fp, sizeof(BITMAPFILEHEADER), SEEK_SET); // 파일 포인터를 이동시킨다
	if (fread(m_pDib, sizeof(BYTE), m_nDibSize, fp) != m_nDibSize) {
		delete[] m_pDib;
		m_pDib = NULL;
		fclose(fp);
		return FALSE;
	}

	// 파일 닫기
	fclose(fp);

	return TRUE;
}

BOOL IppDib::SaveBMP(const char* filename)
{
	if (!IsValid())
		return FALSE;

	FILE* fp;
	fopen_s(&fp, filename, "wb"); // 쓰기모드로 여는것 같다.
	if (!fp)
		return FALSE;

	BITMAPFILEHEADER bmfh;
	bmfh.bfType = DIB_HEADER_MARKER; // 파일의 형태를 지정
	bmfh.bfSize = (DWORD)sizeof(BITMAPFILEHEADER) + m_nDibSize; // 비트맵 파일의 크기를 바이트 단위로 지정
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * GetPaletteNums())); // 구조체와 실제 비트맵 데이터와의 오프셋 값을 가진다

	fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(GetBitmapInfoAddr(), m_nDibSize, 1, fp);

	fclose(fp);

	return TRUE;
}

// 함수 비트맵을 원본 크기 그대로 출력
void IppDib::Draw(HDC hdc, int dx, int dy) 
{
	if (m_pDib == NULL)
		return;

	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	LPVOID lpBits = (LPVOID)GetDIBitsAddr();

	::SetDIBitsToDevice(hdc, // hdc
		dx,					 // DestX
		dy,					 // DestY
		m_nWidth,			 // nSrcWidth
		m_nHeight,			 // nSrcHeight
		0,					 // SrcX
		0,					 // SrcY
		0,					 // nStartScan
		m_nHeight,			 // nNumScan
		lpBits,				 // lpBits
		lpbi,				 // lpBitsInfo
		DIB_RGB_COLORS);	 // wUsage

}

 // 비트맵 원본 자체를 확대 또는 축소하여 출력할 수 있다.
void IppDib::Draw(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop)
{
	Draw(hdc, dx, dy, dw, dh, 0, 0, m_nWidth, m_nHeight, dwRop);
}

 // 원본 비트맵의 부분 영역을 출력할 수 있다.
void IppDib::Draw(HDC hdc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, DWORD dwRop)
{
	if (m_pDib == NULL)
		return;

	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	LPSTR lpDIBBits = (LPSTR)GetDIBitsAddr();

	SetStretchBltMode(hdc, COLORONCOLOR);
	::StretchDIBits(hdc, // hdc
		dx,					 // XDest
		dy,					 // YDest
		dw,      			 // nDestWidth
		dh,					 // nDestHeight
		sx,					 // XSrc
		sy,					 // YSrc
		sw,					 // nSrcWidth
		sh,					 // nSrcHeight
		lpDIBBits,			 // lpBits
		lpbi,				 // lpBitsInfo
		DIB_RGB_COLORS,		 // wUsage
		SRCCOPY);			 // dwROP
}

// IppDib 객체가 가지고 있는 DIB 영상을 Windows 클립보드로 복사한다.
BOOL IppDib::CopyToClipboard()
{
	if (!::OpenClipboard(NULL))
		return FALSE;

	// DIB 전체를 전역 메모리 블럭에 복사
	DWORD dwDibSize = GetDibSize();
	HANDLE hDib = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, dwDibSize); // 해당 메모리를 사용할수 있는 핸들값
	if (hDib == NULL) {
		::CloseClipboard();
		return FALSE;
	}

	LPVOID lpDib = ::GlobalLock((HGLOBAL)hDib); // 실제 사용가능한 메모리의 주소가 반환
	memcpy(lpDib, GetBitmapInfoAddr(), dwDibSize);
	::GlobalUnlock(hDib); // 메모리의 주소를 변경이 가능하도록 한다.

	// 클립보드에 데이터 입력
	::EmptyClipboard();
	::SetClipboardData(CF_DIB, hDib); // 장치 독립 비트맵 설정
	::CloseClipboard();

	return TRUE;
}

// 클립보드로부터 DIB 내용을 불러와서 IppDib 객체로 변환
BOOL IppDib::PasteFromClipboard()
{
	// CF_DIB 타입이 아니면 종료한다.
	if (!::IsClipboardFormatAvailable(CF_DIB))
		return FALSE;

	// 클립보드를 연다.
	if (!::OpenClipboard(NULL))
		return FALSE;

	// 클립보드 내용을 받아 온다.
	HANDLE hDib = ::GetClipboardData(CF_DIB);
	if (hDib == NULL) {
		::CloseClipboard();
		return FALSE;
	}

	// 메모리 블럭의 크기는 DIB 전체 크기와 동일
	DWORD dwSize = (DWORD)::GlobalSize((HGLOBAL)hDib);
	LPVOID lpDib = ::GlobalLock((HGLOBAL)hDib);

	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)lpDib;
	m_nWidth = lpbi->biWidth;
	m_nHeight = lpbi->biHeight;
	m_nBitCount = lpbi->biBitCount;

	DWORD dwWidthStep = (DWORD)((m_nWidth * m_nBitCount / 8 + 3) & ~3);
	DWORD dwSizeImage = m_nHeight * dwWidthStep;

	if (m_nBitCount == 24)
		m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;
	else
		m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << m_nBitCount) + dwSizeImage;

	// 현재 설정된 IppDib 객체가 있다면 삭제한다.
	if (m_pDib)
		DestroyBitmap();

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL) {
		::GlobalUnlock(hDib);
		::CloseClipboard();
		return FALSE;
	}

	memcpy(m_pDib, lpDib, m_nDibSize);
	::GlobalUnlock(hDib);
	::CloseClipboard();

	return TRUE;

} 

IppDib& IppDib::operator=(const IppDib& dib)
{
	if (this == &dib) // 재귀 검사 (자기 자신에게 복사하는 문제점을 방지)
		return *this;

	if (m_pDib)
		delete[] m_pDib;

	m_nWidth = dib.m_nWidth;
	m_nHeight = dib.m_nHeight;
	m_nBitCount = dib.m_nBitCount;
	m_nDibSize = dib.m_nDibSize;
	m_pDib = NULL;

	if (dib.m_pDib != NULL) {
		m_pDib = new BYTE[m_nDibSize];
		memcpy(m_pDib, dib.m_pDib, m_nDibSize);
	}

	return *this;

}

BYTE* IppDib:: GetDIBitsAddr() const
{
	if (m_pDib == NULL)
		return NULL;

	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	return ((BYTE*)m_pDib + lpbmi->biSize + (sizeof(RGBQUAD) * GetPaletteNums()));
}

int IppDib::GetPaletteNums() const
{
	switch (m_nBitCount)
	{
	case 1: return 2;
	case 4: return 16;
	case 8: return 256;
	default: return 0;
	}
}
