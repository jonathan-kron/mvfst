/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <quic/QuicConstants.h>
#include <quic/codec/Types.h>
#include <quic/state/StateData.h>
#include <functional>

namespace quic {

using AckVisitor = std::function<
    void(const OutstandingPacket&, const QuicWriteFrame&, const ReadAckFrame&)>;

/**
 * Processes an ack frame and removes any outstanding packets
 * from the connection that have already been sent.
 *
 * Returns AckEvent with information about what was observed during processing
 */
AckEvent processAckFrame(
    QuicConnectionStateBase& conn,
    PacketNumberSpace pnSpace,
    const ReadAckFrame& ackFrame,
    const AckVisitor& ackVisitor,
    const LossVisitor& lossVisitor,
    const TimePoint& ackReceiveTime);

/**
 * Clears outstanding packets marked as lost that are not likely to be ACKed
 * (have been lost for >= 1 PTO).
 */
void clearOldOutstandingPackets(
    QuicConnectionStateBase& outstandings,
    TimePoint time,
    PacketNumberSpace pnSpace);

/**
 * Visitor function to be invoked when we receive an ACK of the WriteAckFrame
 * that we sent.
 */
void commonAckVisitorForAckFrame(
    AckState& ackState,
    const WriteAckFrame& frame);
} // namespace quic
