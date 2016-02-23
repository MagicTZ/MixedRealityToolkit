//////////////////////////////////////////////////////////////////////////
// AnchorDownloadRequestImpl.h
//
// Copyright (C) 2016 Microsoft Corp.  All Rights Reserved
//////////////////////////////////////////////////////////////////////////

#pragma once

XTOOLS_NAMESPACE_BEGIN

class AnchorDownloadRequestImpl : public AnchorDownloadRequest
{
public:
	AnchorDownloadRequestImpl(const XStringPtr& anchorName, const RoomPtr& room);

	/// Returns the name of the anchor that was requested
	virtual XStringPtr GetAnchorName() const XTOVERRIDE;

	/// Returns the Room that the requested anchor is in
	virtual RoomPtr GetRoom() const XTOVERRIDE;

	/// Returns true as long as the requested anchor data is still downloading
	virtual bool IsDownloading() const XTOVERRIDE;

	/// Cancel the pending download.  If the download has already completed then this does nothing
	virtual void CancelDownload() XTOVERRIDE;

	/// Returns the size of the downloaded data.  Returns 0 until the download completes
	virtual int32 GetDataSize() const XTOVERRIDE;

	/// Copies the downloaded data into the given buffer.  
	/// Returns true on success, false if the data has not completed download or the size of the given buffer is no
	/// big enough to hold the data.  
	virtual bool GetData(byte* data, int32 dataSize) const XTOVERRIDE;

	// Set the downloaded data so that the user can retrieve it by calling GetData
	void SetData(byte* data, int32 dataSize);

private:
	enum Status
	{
		Downloading = 0,
		Downloaded,
		Cancelled
	};

	XStringPtr	m_name;
	RoomPtr		m_room;
	Buffer		m_data;
	Status		m_status;
};

DECLARE_PTR(AnchorDownloadRequestImpl)

XTOOLS_NAMESPACE_END
