#pragma once
#ifndef GPC_CONFIG_H
#define GPC_CONFIG_H

/**
 * @brief When server received master, print out what header it got
 */
#define SERVER_QUERY_LOG
/**
 * @brief Print the progress bar in console when server is download the media from GoPro
 */
#define SERVER_MEDIA_DOWNLOAD_LOG
/**
 * @brief The progress bar wait period time (second)
 */
#define SERVER_MEDIA_DOWNLOAD_PERIOD 1.0

#endif